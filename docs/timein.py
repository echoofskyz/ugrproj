import time

file = open(".\projectlog.txt", "a")
file.write("> " + time.asctime() + "\n")
file.close()
