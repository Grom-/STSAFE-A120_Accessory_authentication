# STSAFE-A120_Multi-step_device_authentication{#STSAFE-A120_Multi-step_device_authentication}

This example illustrates STSAFE-A120 device authentication process using Multi-Step approach . it can be taken as reference for building distant server authentication use cases.      

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Multi-step_device_authentication.png} "STSAFE-A120_Device_authentication Example flowchart" width=5cm

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
                          STSAFE-A120 Multi-Steps Device Authentication Example
----------------------------------------------------------------------------------------------------------------
- This example illustrates STSAFE-A120 device authentication process using Multi-Step approach .
- it can be taken as reference for building distant server authentication use cases
----------------------------------------------------------------------------------------------------------------

## CA self-signed certificate :

         x509 Version: 3
        SerialNumber: 0100000000000001
         Issuer:
                 CountryName: NL
                 OrganizationalName: StMicroelectronics nv
                 CommonName: STM STSAFE-A TEST CA 91
         Subject:
                 CountryName: NL
                 OrganizationalName: StMicroelectronics nv
                 CommonName: STM STSAFE-A TEST CA 91
         Validity:
                 Not Before: 2023-06-26 00:00:00
                 Not After:  2053-06-26 00:00:00
         SignatureAlgorithm: ecdsa-with-SHA256
         tbsSignature: ecdsa-with-SHA256
         EllipticCurve: P-256
         Cert PubKey:
                 X: 2721C562F8F2268FADD3741BC58B98F416C2217AB98BF0DEBF792DB2C72FC2FF
                 Y: A61CA326380CD35D77DB396976364A2368C565F96921BFA1CC3B2A621FAD62B9
         Cert Signature:
                 r: EF1C731964A87379C245A709B336DAF7ED0556BD70937C9FBFA46E2BF4CF7A8F
                 s :4668A08D4B4812DF426865ED2C17B602931D315D49CDD5C674A54E612F82D574
         List of Extensions:
                 BasicConstraints: CA certificate.

## Target STSAFE-Axxx certificate :

         x509 Version: 3
        SerialNumber: 0209FBA403B2E48C7401F1
         Issuer:
                 CountryName: NL
                 OrganizationalName: STMicroelectronics nv
                 CommonName: STM STSAFE-A TEST CA 91
         Subject:
                 CountryName: FR
                 OrganizationalName: STMicroelectronics
                 CommonName: eval4-0209FBA403B2E48C7401F1
         Validity:
                 Not Before: 2023-12-13 00:00:00
                 Not After:  2053-12-13 00:00:00
         SignatureAlgorithm: ecdsa-with-SHA256
         tbsSignature: ecdsa-with-SHA256
         EllipticCurve: P-256
         Cert PubKey:
                 X: 4D1B4E44BC793513D21254FEA18EACAAF2636A5086CA48C2FA6846AD6B37161A
                 Y: 9DFE9598F8703762DF67792B1E595583EB4D8488039C683A81B3A59A71926D67
         Cert Signature:
                 r: 15D93E5BEBE90778DC1C965DB43C9AEC9AF3C06A96AE028CE999D14B57AF4DB4
                 s :BD1A6D816C4EF5591B8919D96839FDB83AAC4F5A53A42671371132BA99D81065
         List of Extensions:
                 BasicConstraints: Not a CA certificate.

## Device Certificate Verified


## Host random challenge :

  0x8C 0x10 0x4A 0x17 0x41 0xE0 0x5B 0x84 0xBC 0x69 0x24 0x30 0xF4 0xF6 0xED 0xD8
  0xEF 0x90 0xEC 0x03 0x6D 0xAC 0xF7 0x42 0xCF 0xE3 0xAF 0x0D 0xBB 0x02 0x34 0x9D

## Device signature over Host challenge:
  0xBF 0x64 0x28 0x56 0x33 0x6F 0xA1 0x4A 0xC3 0x82 0x86 0xF3 0xAC 0xED 0x2D 0xA1
  0xD9 0x27 0x67 0x4F 0xDE 0x7A 0xB8 0xD7 0x8D 0x24 0x0D 0xE0 0x4C 0x8C 0x6C 0xE9
  0x8E 0x93 0xF1 0xC0 0xE7 0xB3 0x65 0x56 0x85 0xC1 0x72 0xAD 0x8F 0x8A 0xA6 0x84
  0x20 0x2D 0x81 0x39 0x4A 0x70 0xBC 0x3D 0xC4 0x5C 0x6C 0x85 0xAF 0xD4 0xE7 0x28

# ## Device Authenticated (Challenge signature verified successfully)
```


Applicative scenario : 

- Authentication from/to remote server 