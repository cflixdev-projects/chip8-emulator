#pragma once
#include "init_chip.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool load_rom(Chip8 *chip8, const char *filename);