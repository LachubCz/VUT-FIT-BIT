import datetime
import time

then = datetime.datetime(2018, 6, 11, 8, 0, 0, 0)

while True:
    time.sleep(1)
    now = datetime.datetime.now()
    print("#######", ((then - now)/38), "#######", end='\r')
