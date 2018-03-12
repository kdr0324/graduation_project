#ifndef _LUT_H_
#define _LUT_H_

#include "Config.h"

extern unsigned char *r_lookup;
extern unsigned char *g_lookup;
extern unsigned char *b_lookup;

extern unsigned char *h_lookup;
extern unsigned char *s_lookup;
extern unsigned char *i_lookup;
extern short *sin_lookup;
extern short *cos_lookup;

#ifdef __cplusplus
extern "C" {
#endif
	void OpenLookUpTable(void);
	void CloseLookUpTable(void);

#ifdef __cplusplus
}
#endif

#endif // _LUT_H_
