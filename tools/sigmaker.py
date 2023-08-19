"""
Extremely stupid sig maker script I use for IDA.

-In IDA, Options, General, set Number of Opcode Bytes to 10.
-Copy the lines you want into the code variable below, e.g.:
.text:00007FF65040DD49 E8 C2 86 F8 02     call    sub_7FF653396410
.text:00007FF65040DD4E 90                 nop
-Run the script
-Copy the output and check in IDA if you find your desired address and no other.
"""
import re

code = """\
paste here
"""

output = ""
lines = code.splitlines()
for line in lines:
    line_match = re.match(r'\.text:[0-9A-F]+ ([0-9A-F][0-9A-F]) ((?:[0-9A-F][0-9A-F] )*).*?$', line)
    if line_match:
        first_byte = line_match.group(1)
        output += first_byte + " "
        other_bytes = line_match.group(2)
        other_bytes_match = re.findall(r'([0-9A-F][0-9A-F] )', other_bytes)
        for i in range(len(other_bytes_match)):
            output += "?? "

print(output)
