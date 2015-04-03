import RadioInterface

pipe = RadioInterface.RadioInterface("edison", True)

rawTemp = 0
node = 2

rawTemp = pipe.pull("boosterpack", node, "temperature")
print("Got raw temp: {0}".format(rawTemp))
