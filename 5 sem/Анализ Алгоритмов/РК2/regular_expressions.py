import re

s = ' 3a [2-9_8] [2-4_4] [2-1] [1,2-3], [2_11,5-3,7_8_2], [22] [16,4]]'

n = int(input('Введите количество строк в тексте: '))
print('Введите текст по строке')
m = []
for i in range (n):
    m.append(str(input('')))
    #print(i,'   ',m[i])

print('\n\n')


for i in range (n):
    print(i+1,'-я строка')
    for k in re.finditer(r'\[((\d+-+){0,}(\d+_+){0,}\d+,){0,}(\d+-+){0,}(\d+_+){0,}\d+\]', m[i]):
        print('   Ссылка ', k[0], ' начинается с позиции ', k.start())
    print('------------------')


#print(s)
#for m in re.finditer(r'\[(\d+-+){0,}(\d+_+){0,}\d+\]', s):
#    print('   Ссылка ', m[0], ' начинается с позиции ', m.start())
#print('------------------')
#for m in re.finditer(r'\[(\d+,){0,}\d+\]', s):
#    print('   Ссылка ', m[0], ' начинается с позиции ', m.start())
#print('------------------')

#for m in re.finditer(r'\[([((\d+-+){0,})((\d+_+){0,})]\d+,){0,}[((\d+-+){0,})((\d+_+){0,})]\d+\]', s):
#    print('   Ссылка ', m[0], ' начинается с позиции ', m.start())
#print('------------------')

