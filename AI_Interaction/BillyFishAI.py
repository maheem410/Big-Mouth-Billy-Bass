from openai import OpenAI
from playsound3 import playsound

#Creates OpenAI client with API key and creates history for memory
client = OpenAI(api_key="Put API Key here") #Took out personal key for security reasons when putting on GitHub
conversation_history = []

#Uses prompt to generate ChatGPT response
def get_chatgpt_response(user_text):
    global conversation_history

    #Add message to memory and only keep last 10 items
    conversation_history.append({"role": "user", "content": user_text})
    conversation_history = conversation_history[-10:]
    
    response = client.chat.completions.create(
        model="gpt-4.1-mini",
        messages= conversation_history
    )
    #Gets reply from AI model and adds it to memory
    reply = response.choices[0].message.content
    conversation_history.append({"role": "assistant", "content": reply})
    
    return reply

#Converts ChatGPT’s text to audio (TTS)
def text_to_speech(text, filename="speech.wav"):
    audio = client.audio.speech.create(
        model="gpt-4o-mini-tts",
        voice="alloy",
        input=text,
        response_format="wav"
    )

    #Saves TTS output to a wav file
    with open(filename, "wb") as f:
        f.write(audio.read())

    return filename

#Play wav file through speakers
def play_audio(filename):
    playsound(filename)

#Main loop!!!
while True:
    print("loop start")
    user_input = input("Input a prompt (type exit to quit): ")

    #Exit condition
    if user_input.lower() in ["quit", "exit"]:
        break

    #Get chatGPT response
    response_text = get_chatgpt_response(user_input)
    print("ChatGPT:", response_text)

    #Convert to speech
    filename = text_to_speech(response_text)

    #Play through speakers
    play_audio(filename)