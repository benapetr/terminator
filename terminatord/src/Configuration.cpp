 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/Configuration.h"

//! Version
string Configuration::Version = "1.0.0.2";

//! Verbosity
unsigned int Configuration::Verbosity = 0;

//! Number of items in ignore list
int Configuration::IgnoreCount = 0;

//! List of uid to ignore
int Configuration::Ignore[800];

//! Value that specifies when processes needs to be killed
unsigned int Configuration::SoftSystemLimitMB = 0;

//! Value that specifies when processes needs to be killed
unsigned int Configuration::HardSystemLimitMB = 0;

string Configuration::ExecPath = "";

bool Configuration::Exec = false;

//! Protect against kernel OOM killer
bool Configuration::ProtectSelf = true;

//! Whether processes should be killed when they exceed certain memory limit
bool Configuration::KillOOM = false;

//! Whether processes should be reported when they exceed certain memory limit
bool Configuration::ReportOOM = true;

bool Configuration::KillSelf = false;

bool Configuration::Swap = true;

//! If root processes can be killed too
bool Configuration::KillRoot = false;

string Configuration::LF = "/var/log/terminator.log";

int Configuration::Logger = 1;

pid_t Configuration::pid = getpid();

//! Limit for processes to kill with sigterm
unsigned int Configuration::HardMemoryLimitMB = 640;

//! Limit for processes to kill
unsigned int Configuration::SoftMemoryLimitMB = 620;

unsigned long Configuration::MemoryTotal = 0;

bool Configuration::DryMode = false;

//! Interval between checks
int Configuration::Interval = 600000;

bool Configuration::Daemon = false;

