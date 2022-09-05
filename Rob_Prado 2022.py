# -*- coding: utf-8 -*-
import serial # pip install pyserial
import threading
import time
import speech_recognition as sr # pip install SpeechRecognition
import pyttsx3 # pip install pyttsx3
import cv2 # pip install opencv-python

#PyAudio https://www.lfd.uci.edu/~gohlke/pythonlibs/#pyaudio 
#"ad3-2.2.1-cp39-cp39-win_amd64.whl"pip  install com o caminho do arquivo baixado cp3.9 = versão do python
import pywhatkit #pip install pywhatkit
import datetime # pip install datetime2
import wikipedia # pip install wikipedia

#   time_func =  time.perf_counter()   # trocar para time_func = time.perf_counter() na linha 264
# em C:\Users\Admin\anaconda3\Lib\site-packages\sqlalchemy\util\compat.py

#chatbot
from chatterbot.trainers import ListTrainer # pip install chatterbot
# caso de erro: No module named 'chatterbot_corpus'
# python -m pip install chatterbot-corpus

# pip install chatterbot-corpus no CMD com administrador

# Porta = 92 WebCam = 145 mic = 88 comandos = 313 voz = 212

from chatterbot import ChatBot

AMGbot = ChatBot("Robô Prado")

# texto inicial, com as conversas o bot vai ficando mais inteligente
conversa1 = ['tudo bem com você', 'comigo tudo bem!', 'oi','olá','olá bom dia', 'bom dia', 'como vai?','estou bem', 'Poderia me dizer como está o tempo lá fora?']
conversa2 = ['Qual é o seu nome?','Robô Prado','quem é você', 'Sou uma plataforma de aprendizagem para Inteligência artificial e internet das coisas ', 'Foi um prazer conhecer você!!','Volte sempre!']
conversa3 = [
'Qual o seu nome?',
'Robô Prado!',
'Quantos anos você tem?',
'tenho anos de pesquisas e desenvolvimento', 
'Aonde você mora?',
'moro em Prado Ferreira', 
'quem te construiu?', 
'Quem me construiu foi Engenheiro Douglas Alexandre junto com os alunos de Prado Ferreira', 
'Quem é você?', 
'Eu sou uma plataforma de desenvolvimento para tecnologias de Inteligência Artificial e internet das coisas, além de ser uma excelente plataforma mecânica para que possa ser experimentados vários tipos de dispositivo', 
'Você é inteligente?', 
'não me considero inteligente estou sempre aprendendo?', 
'você é um homem?', 
'Eu sou um dispositivo de aprendizagem para inteligência artificial', 
'você tem sentimentos?',
'Eu gostaria de ter sentimentos', 
'quem sabe um dia', 
'Você namora?', 
'não, mas gosto da voz da mulher do Google me chama atenção quem sabe um dia eu posso conhecê-la', 
'Você tem família?', 
'minha família é o povo de Prado Ferreira', 
'você é feliz?', 
'se estou funcionando eu sou feliz'
] 
conversa4 = [
'você pode ficar doente?', 
'não, posso apresentar um defeito e aí chama alguém para me ajudar que entenda do assunto de preferência', 
'quanto mais conversar comigo mais inteligente eu fico', 
'Olá', 
'oi', 
'tudo bem?',
'bom dia', 
'bom dia, como você está?', 
'boa tarde', 
'Boa tarde, Tudo bem com você?', 
'Boa noite', 
'ainda não consigo te falar, isso requer umas linhas de códigos', 
'O que você sabe fazer?', 
'ainda sei poucas coisas, estou aprendendo e quero aprender com vocês',
'Quem teve a ideia desse projeto?',
'O ex-prefeito Silvio com o apoio da prefeita María Édina,\
    gostaram da ideia e pediram que eu fosse construido aqui para que pudéssemos trabalhar com os jovens do programa 4.0', 
'Até onde você pode chegar no seu desenvolvimento?', 
'Posso chegar até aonde os alunos me ensinarem e me adaptarem com novas habilidades'

]
treinar = ListTrainer(AMGbot)
treinar.train(conversa1)
treinar.train(conversa2)
treinar.train(conversa3)
treinar.train(conversa4)
r = sr.Recognizer()

mic = sr.Microphone(1) # 0 = microfone embutido

conectado = False

porta = 'COM6' # linux ou mac em geral -> '/dev/ttyS0'
velocidadeBaud = 115200

mensagensRecebidas = 1;
desligarArduinoThread = False
desligarCameraThread = False
desligarVozThread = False

falarTexto = False;
textoRecebido = ""
textoFalado = ""

arduinoFuncionando = True
nuncaTeVi = True;
jaTeVi = False;

try:
    SerialArduino = serial.Serial(porta,velocidadeBaud, timeout = 0.2)
except:
    print("Verificar porta serial ou religar arduino")
    arduinoFuncionando = False

def handle_data(data):
    global mensagensRecebidas, falarTexto, textoRecebido
    print("Recebi " + str(mensagensRecebidas) + ": " + data)
    
    mensagensRecebidas += 1
    textoRecebido = data
    
    falarTexto = True

def read_from_port(ser):
    global conectado, desligarArduinoThread
    
    while not conectado:
        conectado = True

        while True:
           try:
               reading = ser.readline().decode()
           except:
               print("Serial desligada")
           if reading != "":
               handle_data(reading)
           if desligarArduinoThread:
               print("Desligando Arduino")
               break

def conectaCamera():
    global desligarCameraThread, arduinoFuncionando, SerialArduino,\
        nuncaTeVi, jaTeVi
    classificador = cv2.CascadeClassifier(
        "cascades/haarcascade_frontalface_default.xml")
    webCam = cv2.VideoCapture(3) # Seleciona a WebCAm
    while(True):
        conectou, imagem = webCam.read()
        
        imagem = cv2.flip(imagem, 1) # inverte imagem (opcional)
        alturaImagem, larguraImagem = imagem.shape[:2]
        
        converteuCinza = cv2.cvtColor(imagem, cv2.COLOR_BGR2GRAY)
        
        encontrarFaces = classificador.detectMultiScale(converteuCinza,#tamanho   rosto
                                                        scaleFactor=1.5,
                                                        minSize=(50,50),
                                                        maxSize=(100,100))
        cor = (0,0,255)
        for(origemX, origemY, largura, altura) in encontrarFaces:
            cv2.rectangle(imagem,(origemX,origemY),
                          (origemX + largura, origemY + altura),
                          cor,2)
            
            if nuncaTeVi: # quando a camera te ver pela primeira vez
                time.sleep(0.5)
                nuncaTeVi = False
                jaTeVi = True
            
            raio = 4
            centroRosto = (origemX + int(largura/2),origemY + int(altura/2))
            cv2.circle(imagem, centroRosto, raio, cor)
            
            # Normalizar = deixa valores entre zero até um
            normalizarZeroAteUm = int(larguraImagem/2)
            # Correção = transforma valores para 1 até 10
            fatorDeCorrecao = 5
            
            erroCentro = (((centroRosto[0] - (larguraImagem/2))
            /normalizarZeroAteUm) * fatorDeCorrecao)
            
            try:
                if arduinoFuncionando:
                    pass
                # arduino desativado porque o som do motor interfere na voz
                    SerialArduino.write(('servo' + str(erroCentro) + '\n').encode())
            except:
                print("não enviou")
    
        cv2.imshow("Rosto", imagem)
        
        teclou = cv2.waitKey(1) & 0xFF
        if desligarCameraThread:
            webCam.release()
            cv2.destroyAllWindows()
            print("Desligando camera")
            break
 
    
def falar():
    global jaTeVi, falarTexto, textoRecebido, textoFalado, engine
    
    engine = pyttsx3.init()
    
    voices = engine.getProperty('voices')
    engine.setProperty('rate', 150) # velocidade 120 = lento
    contar = 0;
    for vozes in voices: # listar vozes
        print(contar, vozes.name)
        contar+=1
    
    voz =2 #3 IVONA 2 Ricardo - portugalski brazylijski
    engine.setProperty('voice', voices[voz].id)
    
  
    #IVONA VOICE: RICARDO
    #https://harposoftware.com/en/portuguese-brasil/166-ricardo-portuguese-brasilian-voice.html
    #https://kripytonianojarvis.com/site/pre-instalacao/

    while True:          
        if desligarVozThread:
            engine.stop()
            break
        if jaTeVi:

            engine.say("Olá Humano! Tudo bem?")
            engine.runAndWait()
            jaTeVi = False     
        
        if falarTexto:
            if textoRecebido != "":
                engine.say(textoRecebido)
                engine.runAndWait()
                textoRecebido = ""
                falarTexto = True
                
            elif 'teste' in textoRecebido:
                mensagem = """ conseguimos"""
                engine.say(mensagem)
                engine.runAndWait()
                falarTexto = True
                
            elif textoFalado != "":
                resposta = AMGbot.get_response(textoFalado)
                print("Robô Prado: " + str(resposta))
                engine.say(resposta)
                engine.runAndWait()
                textoFalado = ""
            
            falarTexto = False
                       
def desligando():
    global desligarArduinoThread, arduinoFuncionando, \
        SerialArduino, lerSerialThread,  \
        desligarCameraThread, desligarVozThread
    
    desligarArduinoThread = True
    desligarCameraThread = True
    desligarVozThread = True
    
    if arduinoFuncionando:
        SerialArduino.close()
        lerSerialThread.join()
        falarVozThread.join()
        print("Tudo desligado")
 
if arduinoFuncionando:
    try:
        lerSerialThread = threading.Thread(target=read_from_port, args=(SerialArduino,))
        lerSerialThread.start()
    except:
        print("Verificar porta serial ou religar arduino")
        arduinoFuncionando = False
    print("Preparando Arduino")
    time.sleep(2)
    print("Arduino Pronto")
else:
    time.sleep(2)
    print("Arduino não conectou")

ligaCamera = True
if ligaCamera:
    try:
        cameraLigadaThread = threading.Thread(target=conectaCamera)
        cameraLigadaThread.start()
    except:
        print("sem câmera")
        
falarVozes = True
if falarVozes:
    try:
        falarVozThread = threading.Thread(target=falar)
        falarVozThread.start()
    except:
        print("sem mic")

while(nuncaTeVi): # só conversa depois de ver a pessoa
        pass

while (True):
  
    try:
        with mic as fonte:
            r.adjust_for_ambient_noise(fonte)
            print("Fale alguma coisa")
            audio = r.listen(fonte)
            print("Enviando para reconhecimento")
        try:
            text = r.recognize_google(audio, language= "pt-BR").lower()
            print("Você disse: {}".format(text))
                        
            if arduinoFuncionando:
               SerialArduino.write((text + '\n').encode())
               print("Dado enviado")
              
               
               
               
            if ("horas" in text):
                print("Respondendo as horas")
                time = datetime.datetime.now().strftime('%I:%M')
                textoRecebido = 'Sâo exatamente ' + time
                textoFalado =  'Sâo exatamente ' + time
                falarTexto = True
                 
                
            if ("tocar música" in text):
                print("Procurando música")
                song = text.replace('tocar música', '')
                textoRecebido = 'tocando ' + song
                textoFalado =  'tocando ' + song
                falarTexto = True
                pywhatkit.playonyt(song)
                
                
                
                          
            
                
            if ("pesquise por" in text):
                print("Pesquisando...")
                wikipedia.set_lang("pt") # Defina antes
                pesquisa = text.replace('pesquise por', '')
                textoRecebido ='pesquisando por' + pesquisa     
                textoFalado ='' + pesquisa
                falarTexto = True
                
                info = wikipedia.summary(pesquisa, 2)
                textoRecebido = info
                textoFalado =  info
                falarTexto = True
                engine.runAndWait()
                print(info) 
                
            if ("parar música" in text):
                print("Parando música")
                textoRecebido = "parando música"
                textoFalado =  "parando música"
                falarTexto = True
                #browser.close()
                '''
                driver = webdriver.Chrome() 
                url = "https://www.youtube.com/"
                driver.get(url) 
                driver.close() 
                '''
             
            if (text.startswith("desativar")):
                print("Saindo")
                desativando = "Robô Prado desativando."
                textoRecebido = desativando
                textoFalado = desativando
                falarTexto = True
                          
                desligando()
                
                
                break
            
            
            # retirar os textos que são comandos especiais
            if text != "" and not text.startswith("ligar") and \
            not ("pesquise por" in text) and \
            not ("pesquisando por" in text) and \
            not ("horas" in text) and \
            not ("parar música" in text) and \
            not ("apresentação" in text) and \
            not ("tocar música" in text) and \
            not text.startswith("desligar") \
            and not text.startswith("desativar"):
                textoFalado = text
                falarTexto = True
        except:
            print("Não entendi, poderia repetir por favor?\n")
            nentendi = "Não entendi, poderia repetir por favor?"
            textooRecebido = nentendi
            textoFalado = nentendi
            falarTexto = False
            

    except (KeyboardInterrupt, SystemExit):
        print("Apertou Ctrl+C")
        desligando()
        
        break
   
    

   







 

