#ifndef __extract_h
#define __extract_h

/// \file extract.h
/// 
/// This module defines functions for parsing text and callbacks.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#include "store.h"

/// Callback function for use with extractors.
/// \param symbol           the symbol of this currency (e.g. GBP or USD)
/// \param nation           name of the nation that hosts this currency (United States of America)
/// \param conversion_to    the rate for converting to this currency from USD
/// \param conversion_from  the rate for converting to USD from this currency
typedef void (*exchange_rate_callback)(const char*, const char*, double, double);

/// This opaque struct is used to maintain state between initialization and 
/// whatever extraction processes you start.
typedef struct extraction_state_s *extraction_state;

/// Create a blank extraction state.
extraction_state setup_extraction(void);

/// Run an extraction between a given file and a given callback, managed by state.
void extract_from_file(extraction_state extractor, FILE* fp, exchange_rate_callback callback);


#endif