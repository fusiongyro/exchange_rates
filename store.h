#ifndef __store_h
#define __store_h

/// \file store.h
/// 
/// This module defines the functions for opening the store, writing a
/// currency unit to it, and closing it.

/// Opens the storage subsystem. Call it once.
void open_storage(void);

/// Write a currency unit to the subsystem.
void store_exchange_rate(const char* symbol, double exchange_to, double exchange_from);

/// Close the storage subsystem. Call it once for each time you call 'open'.
void close_storage(void);

#endif
