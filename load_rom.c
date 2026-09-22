#include "load_rom.h"

bool load_rom(Chip8 *chip8, const char *filename)
{
    FILE *data = fopen(filename, "rb");
    if (data == NULL)
    {
        printf("Fehler beim öffnen der Datei!\n");
        return false;
    }

    fseek(data, 0, SEEK_END);
    long size = ftell(data);
    rewind(data);

    if (size > (4096 - 0x200))
    {
        printf("Rom zu groß!\n");
        fclose(data);
        return false;
    }

    size_t bytes_read = fread(&chip8->memory[0x200], size, 1, data);

    if (bytes_read != 1)
    {
        printf("Fehler beim Lesen der ROM-Datei!\n");
        fclose(data);
        return false;
    }

    fclose(data);
    printf("ROM erfolgreich geladen! Bytes: %ld\n", size); // Hier eine Ausgabe einbauen!

    return true;
}
