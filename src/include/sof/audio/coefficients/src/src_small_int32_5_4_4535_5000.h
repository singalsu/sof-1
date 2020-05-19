/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2020 Intel Corporation. All rights reserved.
 *
 */

#include <sof/audio/src/src.h>
#include <stdint.h>

const int32_t src_int32_5_4_4535_5000_fir[440] = {
	-83573,
	123255,
	-160503,
	187662,
	-194816,
	169963,
	-99413,
	-31555,
	237777,
	-533049,
	928749,
	-1432306,
	2045568,
	-2763183,
	3571078,
	-4445155,
	5350323,
	-6239964,
	7055936,
	-7729182,
	8180988,
	-8324897,
	8069255,
	-7320302,
	5985695,
	-3978305,
	1220083,
	2354242,
	-6793899,
	12129543,
	-18371801,
	25511269,
	-33520492,
	42358605,
	-51979800,
	62347685,
	-73459618,
	85389914,
	-98372924,
	112981571,
	-130571352,
	154629535,
	-196364057,
	317509286,
	1894871335,
	-79753133,
	-7542746,
	37898401,
	-52096437,
	58989529,
	-61697143,
	61660667,
	-59690841,
	56320619,
	-51944606,
	46879672,
	-41392530,
	35712260,
	-30035688,
	24529420,
	-19330488,
	14546675,
	-10257080,
	6513231,
	-3340841,
	742224,
	1300724,
	-2823102,
	3873307,
	-4509233,
	4794542,
	-4795165,
	4576168,
	-4199091,
	3719826,
	-3187081,
	2641422,
	-2114863,
	1630962,
	-1205330,
	846469,
	-556843,
	334087,
	-172257,
	63037,
	3152,
	-36198,
	45533,
	-104602,
	168065,
	-243920,
	327919,
	-413003,
	489008,
	-542547,
	557143,
	-513626,
	390850,
	-166736,
	-180367,
	670033,
	-1317870,
	2133467,
	-3118282,
	4263577,
	-5548534,
	6938669,
	-8384681,
	9821846,
	-11170022,
	12334354,
	-13206648,
	13667413,
	-13588439,
	12835765,
	-11272797,
	8763226,
	-5173336,
	373069,
	5764967,
	-13369812,
	22581761,
	-33570694,
	46569474,
	-61935137,
	80265665,
	-102639004,
	131154423,
	-170345927,
	231725796,
	-354343580,
	801509473,
	1676815034,
	-331091304,
	150372751,
	-76520609,
	35787473,
	-10114691,
	-7092514,
	18840204,
	-26726422,
	31712691,
	-34443350,
	35392372,
	-34935610,
	33387666,
	-31020803,
	28074354,
	-24759028,
	21258568,
	-17730192,
	14304652,
	-11086443,
	8154428,
	-5563034,
	3344031,
	-1508866,
	51442,
	1048791,
	-1823568,
	2312541,
	-2560130,
	2612639,
	-2515737,
	2312365,
	-2041140,
	1735242,
	-1421782,
	1121614,
	-849528,
	614741,
	-421631,
	270600,
	-159015,
	82139,
	-33996,
	-88646,
	156214,
	-248013,
	364876,
	-505192,
	664212,
	-833445,
	1000178,
	-1147218,
	1252903,
	-1291443,
	1233643,
	-1048021,
	702331,
	-165473,
	-590276,
	1586764,
	-2837331,
	4344126,
	-6095545,
	8063911,
	-10203541,
	12449319,
	-14715871,
	16897394,
	-18868139,
	20483463,
	-21581297,
	21983732,
	-21498284,
	19918160,
	-17020486,
	12560868,
	-6261566,
	-2211506,
	13291980,
	-27594850,
	46077612,
	-70380387,
	103621567,
	-152557355,
	234920355,
	-416100017,
	1285934782,
	1285934782,
	-416100017,
	234920355,
	-152557355,
	103621567,
	-70380387,
	46077612,
	-27594850,
	13291980,
	-2211506,
	-6261566,
	12560868,
	-17020486,
	19918160,
	-21498284,
	21983732,
	-21581297,
	20483463,
	-18868139,
	16897394,
	-14715871,
	12449319,
	-10203541,
	8063911,
	-6095545,
	4344126,
	-2837331,
	1586764,
	-590276,
	-165473,
	702331,
	-1048021,
	1233643,
	-1291443,
	1252903,
	-1147218,
	1000178,
	-833445,
	664212,
	-505192,
	364876,
	-248013,
	156214,
	-88646,
	-33996,
	82139,
	-159015,
	270600,
	-421631,
	614741,
	-849528,
	1121614,
	-1421782,
	1735242,
	-2041140,
	2312365,
	-2515737,
	2612639,
	-2560130,
	2312541,
	-1823568,
	1048791,
	51442,
	-1508866,
	3344031,
	-5563034,
	8154428,
	-11086443,
	14304652,
	-17730192,
	21258568,
	-24759028,
	28074354,
	-31020803,
	33387666,
	-34935610,
	35392372,
	-34443350,
	31712691,
	-26726422,
	18840204,
	-7092514,
	-10114691,
	35787473,
	-76520609,
	150372751,
	-331091304,
	1676815034,
	801509473,
	-354343580,
	231725796,
	-170345927,
	131154423,
	-102639004,
	80265665,
	-61935137,
	46569474,
	-33570694,
	22581761,
	-13369812,
	5764967,
	373069,
	-5173336,
	8763226,
	-11272797,
	12835765,
	-13588439,
	13667413,
	-13206648,
	12334354,
	-11170022,
	9821846,
	-8384681,
	6938669,
	-5548534,
	4263577,
	-3118282,
	2133467,
	-1317870,
	670033,
	-180367,
	-166736,
	390850,
	-513626,
	557143,
	-542547,
	489008,
	-413003,
	327919,
	-243920,
	168065,
	-104602,
	45533,
	-36198,
	3152,
	63037,
	-172257,
	334087,
	-556843,
	846469,
	-1205330,
	1630962,
	-2114863,
	2641422,
	-3187081,
	3719826,
	-4199091,
	4576168,
	-4795165,
	4794542,
	-4509233,
	3873307,
	-2823102,
	1300724,
	742224,
	-3340841,
	6513231,
	-10257080,
	14546675,
	-19330488,
	24529420,
	-30035688,
	35712260,
	-41392530,
	46879672,
	-51944606,
	56320619,
	-59690841,
	61660667,
	-61697143,
	58989529,
	-52096437,
	37898401,
	-7542746,
	-79753133,
	1894871335,
	317509286,
	-196364057,
	154629535,
	-130571352,
	112981571,
	-98372924,
	85389914,
	-73459618,
	62347685,
	-51979800,
	42358605,
	-33520492,
	25511269,
	-18371801,
	12129543,
	-6793899,
	2354242,
	1220083,
	-3978305,
	5985695,
	-7320302,
	8069255,
	-8324897,
	8180988,
	-7729182,
	7055936,
	-6239964,
	5350323,
	-4445155,
	3571078,
	-2763183,
	2045568,
	-1432306,
	928749,
	-533049,
	237777,
	-31555,
	-99413,
	169963,
	-194816,
	187662,
	-160503,
	123255,
	-83573

};

struct src_stage src_int32_5_4_4535_5000 = {
	3, 4, 5, 88, 440, 4, 5, 0, 0,
	src_int32_5_4_4535_5000_fir};
