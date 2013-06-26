#include "../include/Configuration.h"

//! Version
string Configuration::Version = "1.0.0.0";

//! Verbosity
unsigned int Configuration::Verbosity = 0;

//! Whether processes should be killed when they exceed certain memory limit
bool Configuration::KillOOM = false;

//! Whether processes should be reported when they exceed certain memory limit
bool Configuration::ReportOOM = true;

//! If root processes can be killed too
bool Configuration::KillRoot = false;

unsigned int Configuration::MemoryLimitMB = 64;

int Configuration::Interval = 60000;

bool Configuration::Daemon = false;
