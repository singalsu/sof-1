# Low Latency Passthrough with volume Pipeline and PCM
#
# Pipeline Endpoints for connection are :-
#
#  host PCM_P --> B0 --> EQ_IIR 0 --> B1  --> EQ_FIR 0 --> B2
#    --> Volume 0 --> B3 --> sink DAI0

# Include topology builder
include(`utils.m4')
include(`buffer.m4')
include(`pcm.m4')
include(`pga.m4')
include(`dai.m4')
include(`mixercontrol.m4')
include(`bytecontrol.m4')
include(`pipeline.m4')
include(`eq_iir.m4')
include(`eq_fir.m4')

#
# Controls
#
# Volume Mixer control with max value of 32
C_CONTROLMIXER(Master Playback Volume, PIPELINE_ID,
	CONTROLMIXER_OPS(volsw,
		256 binds the mixer control to volume get/put handlers,
		256, 256),
	CONTROLMIXER_MAX(, 32),
	false,
	CONTROLMIXER_TLV(TLV 32 steps from -64dB to 0dB for 2dB, vtlv_m64s2),
	Channel register and shift for Front Left/Right,
	LIST(`	', KCONTROL_CHANNEL(FL, 1, 0), KCONTROL_CHANNEL(FR, 1, 1)))


#
# Volume configuration
#

define(MY_PGA_TOKENS, concat(`pga_tokens_', PIPELINE_ID))
define(MY_PGA_CONF, concat(`pga_conf_', PIPELINE_ID))

W_VENDORTUPLES(MY_PGA_TOKENS, sof_volume_tokens,
LIST(`		', `SOF_TKN_VOLUME_RAMP_STEP_TYPE	"0"'
     `		', `SOF_TKN_VOLUME_RAMP_STEP_MS		"250"'))

W_DATA(MY_PGA_CONF, MY_PGA_TOKENS)

#
# IIR EQ
#

define(MY_EQIIR_COEF, concat(`eqiir_coef_', PIPELINE_ID))
define(MY_EQIIR_PRIV, concat(`eqiir_priv_', PIPELINE_ID))

ifelse(PIPELINE_FILTER1, `',
	`include(`eq_iir_coef_flat.m4')',
	`include(PIPELINE_FILTER1)'
)

# EQ Bytes control with max value of 255
C_CONTROLBYTES(MY_EQIIR_COEF, PIPELINE_ID,
	CONTROLBYTES_OPS(bytes,
		258 binds the mixer control to bytes get/put handlers, 258, 258),
	CONTROLBYTES_EXTOPS(258 binds the mixer control to bytes get/put handlers,
		258, 258),
	, , ,
	CONTROLBYTES_MAX(, 1024),
	,
	MY_EQIIR_PRIV)

#
# FIR EQ
#

define(MY_EQFIR_COEF, concat(`eqfir_coef_', PIPELINE_ID))
define(MY_EQFIR_PRIV, concat(`eqfir_priv_', PIPELINE_ID))

ifelse(PIPELINE_FILTER2, `',
	`include(`eq_fir_coef_flat.m4')',
	`include(PIPELINE_FILTER2)'
)

# EQ Bytes control with max value of 255
C_CONTROLBYTES(MY_EQFIR_COEF, PIPELINE_ID,
	CONTROLBYTES_OPS(bytes,
		258 binds the mixer control to bytes get/put handlers, 258, 258),
	CONTROLBYTES_EXTOPS(258 binds the mixer control to bytes get/put handlers,
		258, 258),
	, , ,
	CONTROLBYTES_MAX(, 4096),
	,
	MY_EQFIR_PRIV)

#
# Components and Buffers
#

# Host "Passthrough Playback" PCM
# with 2 sink and 0 source periods
W_PCM_PLAYBACK(PCM_ID, Passthrough Playback, 2, 0)

# "EQ 0" has 2 sink period and 2 source periods
W_EQ_IIR(0, PIPELINE_FORMAT, 2, 2, LIST(`		', "MY_EQIIR_COEF"))

# "EQ 0" has 2 sink period and 2 source periods
W_EQ_FIR(0, PIPELINE_FORMAT, 2, 2, LIST(`		', "MY_EQFIR_COEF"))

# "Volume" has x sink and 2 source periods
W_PGA(0, PIPELINE_FORMAT, DAI_PERIODS, 2, MY_PGA_CONF,
	LIST(`		', "PIPELINE_ID Master Playback Volume"))

# Playback Buffers
W_BUFFER(0, COMP_BUFFER_SIZE(2,
	COMP_SAMPLE_SIZE(PIPELINE_FORMAT), PIPELINE_CHANNELS,
	COMP_PERIOD_FRAMES(PCM_MAX_RATE, SCHEDULE_PERIOD)),
	PLATFORM_HOST_MEM_CAP)
W_BUFFER(1, COMP_BUFFER_SIZE(2,
	COMP_SAMPLE_SIZE(PIPELINE_FORMAT), PIPELINE_CHANNELS,
	COMP_PERIOD_FRAMES(PCM_MAX_RATE, SCHEDULE_PERIOD)),
	PLATFORM_HOST_MEM_CAP)
W_BUFFER(2, COMP_BUFFER_SIZE(2,
	COMP_SAMPLE_SIZE(PIPELINE_FORMAT), PIPELINE_CHANNELS,
	COMP_PERIOD_FRAMES(PCM_MAX_RATE, SCHEDULE_PERIOD)),
	PLATFORM_HOST_MEM_CAP)
W_BUFFER(3, COMP_BUFFER_SIZE(DAI_PERIODS,
	COMP_SAMPLE_SIZE(DAI_FORMAT), PIPELINE_CHANNELS,
	COMP_PERIOD_FRAMES(PCM_MAX_RATE, SCHEDULE_PERIOD)),
	PLATFORM_DAI_MEM_CAP)

#
# Pipeline Graph
#
#  host host PCM_P --> B0 --> EQ_IIR 0 --> B1  --> EQ_FIR 0 --> B2
#    --> Volume 0 --> B3 --> sink DAI0

P_GRAPH(pipe-pass-vol-playback-PIPELINE_ID, PIPELINE_ID,
	LIST(`		',
	`dapm(N_BUFFER(0), N_PCMP(PCM_ID))',
	`dapm(N_EQ_IIR(0), N_BUFFER(0))',
	`dapm(N_BUFFER(1), N_EQ_IIR(0))',
	`dapm(N_EQ_FIR(0), N_BUFFER(1))',
	`dapm(N_BUFFER(2), N_EQ_FIR(0))',
	`dapm(N_PGA(0), N_BUFFER(2))',
	`dapm(N_BUFFER(3), N_PGA(0))'))

#
# Pipeline Source and Sinks
#
indir(`define', concat(`PIPELINE_SOURCE_', PIPELINE_ID), N_BUFFER(3))
indir(`define', concat(`PIPELINE_PCM_', PIPELINE_ID), Passthrough Playback PCM_ID)

#
# PCM Configuration

#
PCM_CAPABILITIES(Passthrough Playback PCM_ID, `S32_LE,S24_LE,S16_LE',
	PCM_MIN_RATE, PCM_MAX_RATE, 2, PIPELINE_CHANNELS,
	2, 16, 192, 16384, 65536, 65536)

undefine(`MY_PGA_TOKENS')
undefine(`MY_PGA_CONF')
undefine(`MY_EQIIR_COEF')
undefine(`MY_EQIIR_PRIV')
undefine(`MY_EQFIR_COEF')
undefine(`MY_EQFIR_PRIV')
