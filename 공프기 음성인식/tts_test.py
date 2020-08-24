import pyttsx3
engine = pyttsx3.init()

while True:
    a = input("말할 말을 입력해주세요 : ")
    engine.say(a);
    engine.runAndWait()
    if a =='끝':
        break

print('종료합니다.')
