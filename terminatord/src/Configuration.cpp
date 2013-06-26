#include "../include/Configuration.h"

//! Version
string Configuration::Version = "1.0.0.0";

//! Verbosity
unsigned int Configuration::Verbosity = 0;

//! Whether processes should be killed when they exceed certain memory limit
bool Configuration::KillOOM = false;

//! Whether processes should be reported when they exceed certain memory limit
bool Configuration::ReportOOM = true;

unsigned int Configuration::MemoryLimitMB = 64;

bool Configuration::Daemon = false;
