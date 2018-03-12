#include "stdafx.h"

#include "LUT.h"

unsigned char *r_lookup;
unsigned char *g_lookup;
unsigned char *b_lookup;

unsigned char *h_lookup;
unsigned char *s_lookup;
unsigned char *i_lookup;
short *sin_lookup;
short *cos_lookup;

#ifndef _WIN32
const char LUT_PATH[] = "LUT/";
#else
const char LUT_PATH[] = "../LUT/";
#endif

void OpenLookUpTable(void)
{
	FILE *fp = NULL;
	char filename[1024];
	const char lut_names[][30] = {
		"hue.bin", "saturation.bin", "intensity.bin", 
		"red.bin", "green.bin", "blue.bin", 
		"sin.bin", "cos.bin"
	};	

	sprintf(filename, "%s%s", LUT_PATH, lut_names[0]);
	fp = fopen(filename, "rb");
	h_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(h_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[1]);
	fp = fopen(filename, "rb");
	s_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(s_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[2]);
	fp = fopen(filename, "rb");
	i_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(i_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[3]);
	fp = fopen(filename, "rb");
	r_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(r_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[4]);
	fp = fopen(filename, "rb");
	g_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(g_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[5]);
	fp = fopen(filename, "rb");
	b_lookup = (unsigned char *)malloc(sizeof(unsigned char) * 65536);
	fread(b_lookup, sizeof(unsigned char), 65536, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[6]);
	fp = fopen(filename, "rb");
	sin_lookup = (short *)malloc(sizeof(short) * 180);
	fread(sin_lookup, sizeof(short), 180, fp);
	fclose(fp);

	sprintf(filename, "%s%s", LUT_PATH, lut_names[7]);
	fp = fopen(filename, "rb");
	cos_lookup = (short *)malloc(sizeof(short) * 180);
	fread(cos_lookup, sizeof(short), 180, fp);
	fclose(fp);
}

void CloseLookUpTable(void)
{
	free(h_lookup);
	free(s_lookup);
	free(i_lookup);
	free(r_lookup);
	free(g_lookup);
	free(b_lookup);
	free(sin_lookup);
	free(cos_lookup);
}
