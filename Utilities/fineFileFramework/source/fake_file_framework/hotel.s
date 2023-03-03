
        AREA    fff_mem_file, DATA, READONLY
        EXPORT  HOTEL_MP3_ROM

; Includes the binary file MyBinFile1.bin from the current source folder
HOTEL_MP3_ROM
        INCBIN  .\hotel.mp3
HOTEL_MP3_ROM_End

; Use a relative or absolute path to other folders if necessary
;       INCBIN  c:\project\MyBinFile1.bin
; Add further binary files to merge them if necessary
;       INCBIN  MyBinFile2.bin

        ALIGN 4
; define a constant which contains the size of the image above
HOTEL_MP3_ROM_Length
        DCD     HOTEL_MP3_ROM_End - HOTEL_MP3_ROM

        EXPORT  HOTEL_MP3_ROM_Length

        END