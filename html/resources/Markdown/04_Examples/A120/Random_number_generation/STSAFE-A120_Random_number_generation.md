# STSAFE-A120_Random_number_generation{#STSAFE-A120_Random_number_generation}

This example illustrates a random number generation using STSAFE-A120 Secure Element.
It uses the STSAFE Middleware APIs to generate a random number.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Random_number_generation.png} "STSAFE-A120_Random_number_generation Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
    :Generate random number with STSAFE-A120]
	:Print the random number]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
----------------------------------------------------------------------------------------------------------------
-                                  STSAFE-A120 Random number generation example                                -
----------------------------------------------------------------------------------------------------------------
-
- description :
- This examples illustrates how to use generate random commands of a target STSAFE-A120 device
-
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - stse_generate_random  :

  0xAC 0x0D 0xE0 0x0C 0xB4 0x6C 0x15 0x97 0x73 0xA2 0x76 0xDA 0x34 0x57 0x07 0xD1
  0xA1 0xBB 0x9B 0x11 0x6B 0x6C 0x8F 0xFF 0x26 0x26 0x02 0x6E 0x41 0x98 0x19 0x53
  0x5F 0x27 0x5D 0xBB 0xF8 0x29 0xF4 0x4E 0x02 0x7C 0xAA 0x48 0x8E 0xD2 0x81 0xC8
  0x3F 0xC3 0xAD 0xEC 0x44 0x5D 0x5C 0xB2 0x75 0x8A 0xAB 0xCC 0x00 0x5A 0x64 0x90
```


Applicative scenario : 

- Symmetric key generation
- Private key generation (public key need to be computed separatly)
- Authentication challenge 