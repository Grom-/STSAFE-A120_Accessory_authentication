# STSAFE-A110_Symmetric_key_AES-128_CMAC{#STSAFE-A110_Symmetric_key_AES-128_CMAC}

This example illustrates a symmetric key (AES-128 CMAC) usage in STSAFE-A110 Secure Element.
It uses the STSAFE Middleware APIs to compute then verify a MAC.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A110_Symmetric_key_AES-128_CMAC.png} "STSAFE-A110_Symmetric_key_AES-128_CMAC Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
	:Print symmetric key table info]
	:Compute MAC over message with the STSAFE-A API]
	:Verify MAC over message with the STSAFE-A API]
	while (while(1))
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
 ----------------------------------------------------------------------------------------------------------------
 -                              STSAFE-A110 Symmetric key cryptography example                                  -
 -                                              AES 128 CMAC                                                    -
 ----------------------------------------------------------------------------------------------------------------
 -
 - description :
 - This examples illustrates how to perform AES CMAC cryptographic operations in STSAFE-A110 secure element
 -
 - Note :
 -     o Device with accessible symmetric key (i.e. STSAFE-A110 SPL03) is required to run this example
 -     o Host key slot must be provisioned using STSAFE-A110_Host_key_provisioning_example
 -
 ----------------------------------------------------------------------------------------------------------------

 - Initialize target device


 -------------------------------------+-----------------------------------------+------------------------------------------------------+
                                      |                 KEY USAGE               |                     PARAMETERS                       |
 ------+------+----------+------------+---------+-----------+---------+---------+-------------+--------------+------------+------------+
  SLOT | LOCK | KEY TYPE | MODE OF OP | MAC GEN | MAC VERIF | ENCRYPT | DECRYPT | MIN MAC LEN | AUTH TAG LEN | CNT OFFSET |   CNT VAL  |
 ------+------+----------+------------+---------+-----------+---------+---------+-------------+--------------+------------+------------+
   00  |   -  | AES 128  |    CCM*    |    -    |     -     |    x    |    x    |      -      |    0x00      |     -      |      -     |
   01  |   -  | AES 128  |    CCM*    |    -    |     -     |    x    |    x    |      -      |    0x04      |    000     | 0x00000000 |
   02  |   -  | AES 128  |    ECB     |    -    |     -     |    x    |    x    |      -      |      -       |     -      |      -     |
   03  |   -  | AES 128  |    CMAC    |    x    |     x     |    -    |    -    |    0x04     |      -       |     -      |      -     |
   04  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   05  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   06  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   07  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   08  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   09  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   10  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   11  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   12  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   13  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   14  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |
   15  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |      -     |

 - Plain text Message :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Message MAC :

  0xE4 0xA9 0x0B 0xBE

 - Message MAC verification : PASS
```


Applicative scenario : 

- Symmetric message authentication (MAC)