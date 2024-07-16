# STSAFE-A110_Multi-step_device_authentication{#STSAFE-A110_Multi-step_device_authentication}

This example illustrates STSAFE-A110 device authentication process using Multi-Step approach . it can be taken as reference for building distant server authentication use cases.      

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A110_Multi-step_device_authentication.png} "STSAFE-A110_Device_authentication Example flowchart" width=5cm

	:MAIN;
	:Initialize Apps terminal (baudrate = 115200)]
	:Print example title and instructions]
	:ret = <b>stse_init</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif
	
	:ret = <b>stse_certificate_parse</b>(CA certificate)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:<b>stse_certificate_print_parsed_cert</b> (parsed CA certificate)|

	:ret = <b>stse_get_device_certificate</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:ret = <b>stse_certificate_parse</b> (device certificate)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:<b>stse_certificate_print_parsed_cert</b> (parsed device certificate)|
	
	:ret = <b>stse_certificate_is_parent</b> (parsed CA certificate,parsed device certificate)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:keytype = <b>stse_certificate_get_key_type (parsed device certificate)</b>|
	:Generate challenge (keytype) ]
	:ret = <b>stse_ecc_generate_signature</b> (keytype)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR ]
		-[hidden]->
		detach
	endif

	:Print signature ]
	:ret=<b>stse_certificate_verify_signature</b> (keytype) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print Error ]
		-[hidden]->
		detach
	endif
	:Print "<b>Device Authenticated</b>" ]
	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

API used in the example :

- \ref stse_init
- \ref stse_certificate_parse
- \ref stse_certificate_print_parsed_cert
- \ref stse_certificate_get_key_type
- \ref stse_certificate_verify_signature
- \ref stse_certificate_is_parent
- \ref stse_ecc_generate_signature


When executed following logs can be seen on the host attached computer terminal window \n


```
----------------------------------------------------------------------------------------------------------------
                          STSE Multi-Steps Basic Authentication Example
----------------------------------------------------------------------------------------------------------------
-
- Instructions :
- Press the STM32 Nucleo USER button to update and read memory of the STSE-Axxx device
-
----------------------------------------------------------------------------------------------------------------

## CA self-signed certificate :

         x509 Version: 3
        SerialNumber: 01
         Issuer:
                 CountryName: NL
                 OrganizationalName: STMicroelectronics nv
                 CommonName: STM STSAFE-A PROD CA 01
         Subject:
                 CountryName: NL
                 OrganizationalName: STMicroelectronics nv
                 CommonName: STM STSAFE-A PROD CA 01
         Validity:
                 Not Before: 2018-07-27 00:00:00
                 Not After:  2048-07-27 00:00:00
         SignatureAlgorithm: ecdsa-with-SHA256
         tbsSignature: ecdsa-with-SHA256
         EllipticCurve: P-256
         Cert PubKey:
                 X: 82194F26CCA36E0E82195CE66658EC64A466922F58C9E64B5DE1A29E7F39863D
                 Y: 042692E4C8AC79F96D2FED52774D52819539F21F3ECD1938F83D70AEE09CCD8D
         Cert Signature:
                 r: 6EE5433247AC7234FC9D175AA51E83276901ADEC1F005E371F40734DE38CC52E
                 s :B1D9516AAD9A3E86D22B8E3B3BD0146FABB9B922F0452634FE927FF5D636CD90
         List of Extensions:
                 BasicConstraints: Critical. CA certificate.

## Target STSAFE-Axxx certificate :

         x509 Version: 3
        SerialNumber: 0209B0846159E439520139
         Issuer:
                 CountryName: NL
                 OrganizationalName: STMicroelectronics nv
                 CommonName: STM STSAFE-A PROD CA 01
         Subject:
                 CountryName: FR
                 OrganizationalName: STMicroelectronics
                 CommonName: eval3-0209B0846159E439520139
         Validity:
                 Not Before: 2021-03-05 00:00:00
                 Not After:  2051-03-05 00:00:00
         SignatureAlgorithm: ecdsa-with-SHA256
         tbsSignature: ecdsa-with-SHA256
         EllipticCurve: P-256
         Cert PubKey:
                 X: 0FA0F71E23AA2D360AC0E1CB86D23529D38F95ABEA07723EDD587F1C50B66596
                 Y: 0EB0D1F107E07DA054AAFB5E9BA857199DD5BB7F55426131621D40DB132C3CDE
         Cert Signature:
                 r: E977AF4CA3238895C98614C90C4BA9E469216047371F0A9E77741664CCE90BD2
                 s :2AD6BE5394D2375ED112971FE65FF703B33C2BF607FF8A54663C1BB3AA737339
         List of Extensions:

## Device Certificate Verified


## Host random challenge :

  0x59 0x1A 0x35 0x68 0xFD 0xFC 0xCF 0x1B 0x35 0x50 0xBA 0x79 0xE9 0xD1 0x72 0x76
  0x34 0x9E 0xBF 0xC6 0x59 0x0D 0xA6 0x6E 0x6A 0x1D 0x58 0x25 0xFD 0xC3 0x21 0xEE

## Device signature over Host challenge:
  0x4E 0x1E 0x5A 0xA1 0x1E 0x8F 0x62 0xAE 0xC2 0x87 0xAB 0xEC 0xAA 0xF5 0xB8 0xFE
  0x51 0x21 0x41 0xAD 0x2C 0x6D 0x25 0x1B 0x59 0xCF 0xC3 0x25 0x15 0x96 0x48 0x8F
  0x75 0x44 0x7B 0xB9 0xA8 0x38 0xFB 0x30 0xB1 0xC9 0x5E 0x7C 0x09 0x28 0x60 0x96
  0x8C 0x19 0xB4 0x74 0x65 0x29 0xF9 0x03 0x84 0xE0 0xCD 0x0C 0x1A 0xB1 0x41 0x64

# ## Device Authenticated (Challenge signature verified successfully)
```


Applicative scenario : 

- Authentication from/to remote server 