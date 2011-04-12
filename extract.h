#ifndef __extract_h
#define __extract_h

extern char *strdup(const char*);

/// \file extract.h
/// 
/// This module defines functions for parsing text and callbacks.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pcre.h>

#include "store.h"

/// Callback function for use with extractors.
/// \param symbol           the symbol of this currency (e.g. GBP or USD)
/// \param conversion_to    the rate for converting to this currency from USD
/// \param conversion_from  the rate for converting to USD from this currency
typedef void (*exchange_rate_cb)(const char*, double, double);

/// This opaque struct is used to maintain state between initialization and 
/// whatever extraction processes you start.
typedef struct extractor_s *extractor;

/// Create a blank extraction state.
extractor make_extractor(void);

/// Run an extraction between a given file and a given callback, managed by 
/// state.
void extract_from_file(extractor state, FILE* fp, exchange_rate_cb callback);


/// Free the extraction state.
void free_extractor(extractor);

#endif
