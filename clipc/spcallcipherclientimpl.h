#pragma once

#include <Windows.h>
#include <intrin.h>
#include "defs.h"

namespace WARBIRD
{
	static int EncryptBlock( ULONG64* Data, SIZE_T Size, ULONG64 EncryptionKey, ULONG64** Encrypted )
	{
        int v193 = -1;
        unsigned int v194 = 0;
        int v208 = 0;
        int v211 = 0;

		uint64_t v207 = (uint64_t)Size >> 3;
		if ( v207 )
		{
			uint8_t* v206 = (uint8_t*) Data + 2;
			int v209 = WORD2(EncryptionKey);
			unsigned int v210 = HIDWORD(EncryptionKey) ^ 0xB1B54A58; // ^ LOWORD(EncryptionKey)?
			int v1654 = WORD1(EncryptionKey);

            // parts of the cipher use the EncryptionKey parts (0x4A58, 0xB1B54A58) etc
            // TODO fix inlined values with macro'd encryption key WORDs

			uint8_t* BlockOffset = (uint8_t*)Encrypted + 7;
			do
			{
                
                int v212 = v206[1] | ((*v206 | ((*(v206 - 1) | (*(v206 - 2) << 8)) << 8)) << 8);
                int v213 = v206[5] | ((v206[4] | ((v206[3] | (v206[2] << 8)) << 8)) << 8);
                int v214 = v213 ^ v193;
                int v215 = v212 ^ HIDWORD(EncryptionKey) ^ (v214 - 0x4A58) ^ v194;
                int v216 = (__ROR4__(v215, 7)
                    + WORD1(EncryptionKey)
                    * __ROR4__(v215 ^ HIDWORD(EncryptionKey), 15)) ^ v214;
                int v217 = (v209 * __ROR4__(v216 - 1313519016, 9)
                    - __ROR4__(v216, 10)) ^ v215;
                int v218 = (__ROL4__(v217, 5)
                    + HIWORD(EncryptionKey) * __ROL4__(v217 ^ v209, 4)) ^ v216;
                unsigned int v219 = (HIDWORD(EncryptionKey) - (v218 ^ 0xB1B54A58)) ^ v217;
                int v220 = (WORD1(EncryptionKey) * (v219 - 19032) - (v219 >> 6)) ^ v218;
                int v221 = (19032 * (v209 ^ __ROR4__(v220, 15))) ^ v219;
                int v222 = (v209 * (HIWORD(EncryptionKey) + __ROR4__(~v221, 3))) ^ v220;
                int v223 = (v222 - HIDWORD(EncryptionKey) - 19032) ^ v221;
                int v224 = (v1654 * (v223 ^ HIWORD(EncryptionKey))) ^ __ROR4__(v223, 10) ^ v222;
                int v225 = __ROR4__(v224, 3) ^ (v209 * __ROL4__(
                    v224 ^ 0x4A58,
                    6)) ^ v223;
                unsigned int v226 = (19032 * (__ROR4__(v225, 15) - HIWORD(EncryptionKey))) ^ v224;
                unsigned int v227 = (v226 >> 15) ^ (19032 * __ROL4__(v226 - v209, 3)) ^ (v226 >> 1) ^ (19032 * (v226 ^ HIWORD(EncryptionKey))) ^ v225;
                int v228 = (WORD1(EncryptionKey) * (v227 - v209) - (v227 >> 13)) ^ v226;
                int v229 = __ROR4__(v228, 11) ^ (v209
                    * __ROR4__(-1313519016 - v228, 9)) ^ v227;
                int v230 = (v229 - HIWORD(EncryptionKey) + 1313519016) ^ v228;
                int v231 = (19032 * (v230 ^ WORD1(EncryptionKey))
                    - __ROR4__(v230, 7)) ^ v229;
                int v232 = (WORD1(EncryptionKey)
                    * __ROL4__(v231 ^ HIWORD(EncryptionKey), 4)
                    - __ROR4__(v231, 16)) ^ v230;
                int v233 = (__ROR4__(v232, 4)
                    + v209 * __ROR4__(-1313519016 - v232, 10)) ^ v231;
                int v234 = __ROR4__(v233, 9) ^ (HIWORD(EncryptionKey)
                    * __ROR4__(v233 + 1313519016, 4)) ^ v232;
                int v235 = (19032 * __ROL4__(v234 ^ HIDWORD(EncryptionKey), 8)
                    - __ROL4__(v234, 2)) ^ v233;
                unsigned int v236 = (WORD1(EncryptionKey)
                    * __ROR4__(HIDWORD(EncryptionKey) - v235, 11)
                    - __ROR4__(v235, 12)) ^ v234;
                int v237 = (v236 >> 8) ^ (v209 * (WORD1(EncryptionKey) ^ v236)) ^ v235;
                v206 += 8;
                int v238 = v237 ^ v210;
                int v239 = v208 ^ v237;
                v208 = v212;
                int v240 = v211 ^ v238 ^ v236;
                v211 = v213;
                *(BlockOffset - 4) = v239;
                *BlockOffset = v240;
                int v241 = __ROR4__(v239, 8);
                *(BlockOffset - 5) = v241;
                int v242 = __ROR4__(v240, 8);
                *(BlockOffset - 1) = v242;
                int v243 = __ROR4__(v241, 8);
                *(BlockOffset - 6) = v243;
                int v244 = __ROR4__(v242, 8);
                *(BlockOffset - 2) = v244;
                int v245 = __ROR4__(v243, 8);
                int v246 = __ROR4__(v244, 8);
                *(BlockOffset - 7) = v245;
                *(BlockOffset - 3) = v246;
                v193 = __ROR4__(v246, 8);
                v194 = __ROR4__(v245, 8);
                BlockOffset += 8;
                --v207;
			} while (v207);
		}

        return 0;
	}
}