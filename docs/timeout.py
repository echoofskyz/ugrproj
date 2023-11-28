import time

file = open(".\projectlog.txt", "a")
file.write("< " + time.asctime() + "\r\n")
file.close()
