import openpyxl

f = open("raw_data.txt", "wt")
wookbook = openpyxl.load_workbook("labs.xlsx")

ws = wookbook.active

cnt = 0
for row in ws.values:
    if ((row[1] == 'F1-24' and row[3] == 1) or (row[1] == 'I-hpc' and row[3] == 1)):
        f.write('\t'.join([str(x) for x in row][1:6]) + '\n')
        cnt += 1
print(cnt)
f.close()
