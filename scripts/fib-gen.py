t0 = 0
t1 = 1
t2 = 1

print(str(0) + "," + str(t0))
print(str(1) + "," + str(t1))
for i in range(2, 5001):
    t2 = t1 + t0
    print(str(i) + "," + str(t2))
    t0 = t1
    t1 = t2
