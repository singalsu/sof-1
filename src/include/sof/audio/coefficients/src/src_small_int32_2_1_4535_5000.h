/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2020 Intel Corporation. All rights reserved.
 *
 */

#include <sof/audio/src/src.h>
#include <stdint.h>

const int32_t src_int32_2_1_4535_5000_fir[200] = {
	-79638,
	131437,
	-197166,
	275788,
	-364652,
	459231,
	-552929,
	636988,
	-700515,
	730662,
	-712961,
	631836,
	-471281,
	215706,
	149078,
	-634761,
	1249353,
	-1995992,
	2871767,
	-3866638,
	4962504,
	-6132477,
	7340428,
	-8540840,
	9679008,
	-10691608,
	11507628,
	-12049661,
	12235493,
	-11979944,
	11196859,
	-9801137,
	7710649,
	-4847853,
	1140891,
	3476141,
	-9064151,
	15681812,
	-23391031,
	32266598,
	-42412651,
	53990922,
	-67270800,
	82723156,
	-101210403,
	124413719,
	-155939188,
	204829979,
	-301107631,
	635613098,
	1656246671,
	-272718756,
	115329198,
	-53041763,
	19366367,
	1514842,
	-15302727,
	24573012,
	-30682186,
	34435821,
	-36364131,
	36848896,
	-36186538,
	34621026,
	-32361428,
	29591262,
	-26473294,
	23151819,
	-19753572,
	16387985,
	-13147220,
	10106244,
	-7323107,
	4839520,
	-2681745,
	861823,
	620933,
	-1778195,
	2630727,
	-3206397,
	3538178,
	-3662209,
	3615984,
	-3436724,
	3159975,
	-2818473,
	2441271,
	-2053159,
	1674345,
	-1320385,
	1002338,
	-727097,
	497862,
	-314714,
	175240,
	-75182,
	9054,
	29280,
	-46101,
	47425,
	47425,
	-46101,
	29280,
	9054,
	-75182,
	175240,
	-314714,
	497862,
	-727097,
	1002338,
	-1320385,
	1674345,
	-2053159,
	2441271,
	-2818473,
	3159975,
	-3436724,
	3615984,
	-3662209,
	3538178,
	-3206397,
	2630727,
	-1778195,
	620933,
	861823,
	-2681745,
	4839520,
	-7323107,
	10106244,
	-13147220,
	16387985,
	-19753572,
	23151819,
	-26473294,
	29591262,
	-32361428,
	34621026,
	-36186538,
	36848896,
	-36364131,
	34435821,
	-30682186,
	24573012,
	-15302727,
	1514842,
	19366367,
	-53041763,
	115329198,
	-272718756,
	1656246671,
	635613098,
	-301107631,
	204829979,
	-155939188,
	124413719,
	-101210403,
	82723156,
	-67270800,
	53990922,
	-42412651,
	32266598,
	-23391031,
	15681812,
	-9064151,
	3476141,
	1140891,
	-4847853,
	7710649,
	-9801137,
	11196859,
	-11979944,
	12235493,
	-12049661,
	11507628,
	-10691608,
	9679008,
	-8540840,
	7340428,
	-6132477,
	4962504,
	-3866638,
	2871767,
	-1995992,
	1249353,
	-634761,
	149078,
	215706,
	-471281,
	631836,
	-712961,
	730662,
	-700515,
	636988,
	-552929,
	459231,
	-364652,
	275788,
	-197166,
	131437,
	-79638

};

struct src_stage src_int32_2_1_4535_5000 = {
	0, 1, 2, 100, 200, 1, 2, 0, 0,
	src_int32_2_1_4535_5000_fir};
