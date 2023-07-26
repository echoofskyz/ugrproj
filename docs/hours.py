from datetime import datetime, timedelta

file = open(".\projectlog.txt", "r")

total = timedelta(seconds=0)

clock_ins = []
clock_outs = []

def toDT(string):
    return datetime.strptime(string, "%a %b %d %H:%M:%S %Y")

for line in file.readlines():
    if (line[0] == ">"):
        clock_ins.append(toDT(line[2:-1:]))
    elif (line[0] == "<"):
        clock_outs.append(toDT(line[2:-1:]))

file.close()

for in_, out in zip(clock_ins, clock_outs):
    length = out - in_
    print(str(length)[:-3:]+" h:m")
    total += length

print(f"Total: {str(total)[:-3:]} h:m")
input("ENTER to close");
