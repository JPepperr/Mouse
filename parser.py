import openpyxl

f = open("raw_data.txt", "wt")
wookbook = openpyxl.load_workbook("labs.xlsx")

ws = wookbook.active

d = dict()
num = ord('A')
def get_ascii(x):
    x = x.replace("КОЕЦОЫТОГОВРЕМЕИ", "")
    global num
    global d
    nw = ""
    for s in x:
        cur = ord(s)
        if not(cur in d):
            d[cur] = num
            num += 1
        nw += chr(d[cur])
    return nw

cnt = 0
for row in ws.values:
    if ((row[1] == 'F1-24' and row[3] == 1) or (row[1] == 'I-hpc' and row[3] == 1)):
        cur = list([str(x) for x in row][1:6])
        cur[4] = get_ascii(cur[4])
        f.write('\t'.join(cur) + '\n')
        cnt += 1
print(cnt)
for i in d:
    print(chr(i) + ' -> ' + chr(d[i]))
f.close()
