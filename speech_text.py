import speech_recognition as sr
import subprocess
import tempfile

def main():
    f = open('output.txt', 'w')

    # 音声入力
    while True:
        r = sr.Recognizer()
        text = '何かお話しして下さい。'
        with sr.Microphone() as source:
            # print("何かお話しして下さい。")
            audio = r.listen(source)

        try:
            # Google Web Speech APIで音声認識
            text = r.recognize_google(audio, language="ja-JP")
        except sr.UnknownValueError:
            print("Google Web Speech APIは音声を認識できませんでした。")
        except sr.RequestError as e:
            print("GoogleWeb Speech APIに音声認識を要求できませんでした;"
                " {0}".format(e))
        else:
            print(text)
            f.write(text)
            f.write('\n')
        if text == "終わりだよ":
            break
    print("完了。")
    f.close()

if __name__ == '__main__':
    main()
