# *vk_selection::Selection*
Данный объект содержит некоторую выборку Unit (Пользователь, группа и пр.).
Для хранения данных создаётся бинарный файл с расширением *.vks*, следующего формата:
1. **1 байт**: *isInverted* ('1' или '0')
2. **2-5 байт**: *size* (unsigned long)
3. **Каждые последующие 5 байт**: *unitType* (char) и *id* (unsigned long)

Таким образом, файл с 1.000 пользователей будет иметь размер 5 килобайт, а файл с данными о 15.000.000 пользователях *(Самая крупная на текущий момент группа)* - 75 мегабайт.
