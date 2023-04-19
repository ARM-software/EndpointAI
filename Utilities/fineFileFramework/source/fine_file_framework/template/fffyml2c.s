
        AREA    fff_mem_file, DATA, READONLY
        EXPORT  FFFYML2C_ROM_data

; Includes the binary file MyBinFile1.bin from the current source folder
FFFYML2C_ROM_data
        INCBIN  .\fffyml2c.py
FFFYML2C_ROM_end

; Use a relative or absolute path to other folders if necessary
;       INCBIN  c:\project\MyBinFile1.bin
; Add further binary files to merge them if necessary
;       INCBIN  MyBinFile2.bin

        ALIGN 4
; define a constant which contains the size of the image above
FFFYML2C_ROM_size
        DCD     FFFYML2C_ROM_end - FFFYML2C_ROM_data

        EXPORT  FFFYML2C_ROM_size

        END