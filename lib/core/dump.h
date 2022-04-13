#ifndef RELAYX_COLLECTIONS_DUMPING
#define RELAYX_COLLECTIONS_DUMPING

// ----------------------------->>> Macros helpers <<<-----------------------------
#ifdef COLLECTIONS_DUMPING //------------------------------------------------------

#define DUMP_ARG(ARG) , ARG

#else // --------------------------------------------------------------------------

#define DUMP_ARG(ARG) 

#endif // COLLECTIONS_DUMPING -----------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////

#endif // RELAYX_COLLECTIONS_DUMPING