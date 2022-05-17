import matplotlib
from matplotlib import markers
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os
 
dir = 'static/'
for f in os.listdir(dir):
    os.remove(os.path.join(dir, f))

cnt = 0
cnt1 = 0
cnt2 = 0
need1 = False
need2 = False

file = open('plot_data.txt', 'rt')
for line in file:
    cur = line.strip()
    if (cnt1 == 0 and cnt2 == 0):
        if (cnt > 0):
            if (need1 == True):
                plt.plot(x1, y1, label=lab1, marker='o')
            if (need2 == True):
                plt.plot(x2, y2, label=lab2, marker='o')
            plt.xlabel(xlab)
            plt.ylabel(ylab)
            plt.title(title)
            plt.legend()
            plt.yscale('linear')
            name = title + '.png'
            url = dir + name
            plt.savefig(url)
        plt.clf()
        x1 = []
        y1 = []
        x2 = []
        y2 = []
        cnt += 1
        title, xlab, ylab, lab1, lab2, c1, c2 = cur.split(' ')
        cnt1 = int(c1)
        cnt2 = int(c2)
        if cnt1 > 0:
            need1 = True
        else:
            need1 = False
        if cnt2 > 0:
            need2 = True
        else:
            need2 = False
    else:
        curx = float(cur.split(' ')[0])
        cury = float(cur.split(' ')[1])
        if (cnt1 > 0):
            cnt1 -= 1
            x1.append(curx)
            y1.append(cury)
        else:
            cnt2 -= 1
            x2.append(curx)
            y2.append(cury)

if (cnt > 0):
    if (need1 == True):
        plt.plot(x1, y1, label=lab1, marker='o')
    if (need2 == True):
        plt.plot(x2, y2, label=lab2, marker='o')
    plt.xlabel(xlab)
    plt.ylabel(ylab)
    plt.title(title)
    plt.legend()
    plt.yscale('linear')
    name = title + '.png'
    url = dir + name
    plt.savefig(url)

file.close()