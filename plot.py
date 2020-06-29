import re
import numpy as npy
import matplotlib.pyplot as plot


class DataReader:
    def __init__(self, file_name, sample_size, number_of_device, central_node):
        self.file_name = file_name
        self.sample_size = sample_size
        self.reliability = npy.empty(self.sample_size)
        self.latency = npy.empty(self.sample_size)
        self.number_of_device = number_of_device
        self.tempList = [""]
        self.read_file()
        self.reliability[0] = self.number_of_device
        self.latency[0] = 0
        self.central_node = central_node

    def read_file(self):
        f = open(self.file_name, "r")
        for x in f:
            temp = x.strip()
            if len(temp) > 5:
                self.tempList.append(temp)

    def performance(self):
        index = 0
        singal_init_time = 0
        for validValue in self.tempList:
            if "=======================" in validValue:
                self.reliability[index] = int(
                    (self.reliability[index] / self.number_of_device) * 100)
                index = index + 1
                singal_init_time = 0
            res1 = re.match(r"(\d+)	ID:(\d+)	Door: Unlock signal sent", validValue)
            if res1:
                singal_init_time = int(res1.group(1))
                self.latency[index] = 0
            res = re.match(
                r"(\d+)	ID:(\d+)	Device: Unlock signal found", validValue)
            if res:
                self.reliability[index] = self.reliability[index] + 1
                time_diff = (int(res.group(1)) - singal_init_time)
                if time_diff > self.latency[index]:
                    self.latency[index] = time_diff
                    
        for rel in self.latency:
            print(rel)
        return self.reliability

    def get_latency_data(self):
        return self.latency

    def get_reliability_data(self):
        return self.reliability

number_of_packets = 15
packets_array = npy.arange(0, number_of_packets, 1)
pdf, plotter = plot.subplots()
# for dd in packets_array:
#     print(dd)

obj = DataReader(
    "/home/faiz/contiki-ng/examples/project/data.txt", number_of_packets, 3, 3)
rel = obj.performance()


# plotter.set_xlabel("Number of attempt")
# plotter.set_ylabel("Latency(ms)")
# plotter.plot(packets_array, obj.get_latency_data(), label='time')
# plotter.legend()
# pdf.savefig("/home/faiz/contiki-ng/examples/project/latency.png")


plotter.set_xlabel("Number of attempt")
plotter.set_ylabel("Average Reliability(%)")
plotter.plot(packets_array, obj.get_reliability_data(), label='number of IoT device')
plotter.legend()
pdf.savefig("/home/faiz/contiki-ng/examples/project/reliability.png")
