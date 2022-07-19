in_text_file = open("in_test_1.txt", "r")
in_text = in_text_file.read()
in_list = in_text.split()


out_text_file = open("out.txt", "r")
out_text = out_text_file.read()
out_list = out_text.split()

size_of_numbers = []
for i in range(0, len(in_list), 5):
    size_of_numbers.append(len(in_list[i]) + len(in_list[i+2]) + len(in_list[i+4]))
inxd = 0
for i in range(0,len(out_list),6):
  
    outSize_number = len(out_list[i+1]) + len(out_list[i+3]) + len(out_list[i+5])
    if size_of_numbers[inxd] + int(out_list[i]) == outSize_number:
        print("NUMBERS OK")
    else:
        print("NMBERS NOT OK")

    inxd += 1
    if (int(out_list[i+1]) + int(out_list[i+3]) == int(out_list[i+5])):
        print("SUM OK")
    else:
        print("NOT OK SUM")


