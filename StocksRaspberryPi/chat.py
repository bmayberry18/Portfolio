from openai import OpenAI
from moviepy import *
from moviepy.video.tools.subtitles import SubtitlesClip
from PIL import Image, ImageDraw, ImageFont
import os
import time

def generate_response(tickers, all_prompt, individual_prompts, caption_prompts, period="Daily"):
    
    gameplay_folder = "/Users/brooklynmayberry/Documents/GitHub/StocksRaspberryPi/Subway_Surfers/"
    # Get all video files in the folder, assuming they have a common video extension
    gameplay_files = sorted(
        [os.path.join(gameplay_folder, f) for f in os.listdir(gameplay_folder) if f.endswith(('.mp4', '.avi', '.mov'))]
    )
        
    if period == "Daily":
        audio_file_path = "Daily_Audio.mp3"
        response_file_path = "Daily_Response.txt"
        video_file_path = "Daily_Summary.mp4"
    elif period == "Weekly":
        audio_file_path = "Weekly_Audio.mp3"
        response_file_path = "Weekly_Response.txt"
        video_file_path = "Weekly_Summary.mp4"
        
    audio_files = []
    # audio_files.append(audio_file_path)
    
    video_files = []
    # video_files.append(video_file_path)
    
    for ticker in tickers:
        audio_files.append(f"{ticker}_short_audio.mp3")
        video_files.append(f"{ticker}_Video.mp4")
        
        
        
    client = OpenAI(
    api_key=""
    )

    print("Generating text\n")
    # response_text = generate_text(client, all_prompt, response_file_path)
    print("Generating Audio")
    # generate_audio(client, response_text, audio_file_path)
    print("Generating Short Form Videos")
    # generate_short_form(client, individual_prompts, tickers)
    # for file in files:
    #     audio_files.append(file)
        
    print("Generating video")   
    for file in audio_files:
        print(f"Audio File: {file}")
    for video in video_files:
        print(f"Video File: {video}") 
    passed = False
    # generate_video(audio_files, video_files, gameplay_files, passed)
    print("Generating captions")
    generate_caption(client, caption_prompts)


    
def generate_text(client, prompt, file_path):
    
    completion = client.chat.completions.create(
    model='gpt-4o',
    messages=[
        {"role": "developer", 
         "content": "You are a professional analyst for stocks so no emojis. Always read tickers as their full name, ex: AAPL is Apple and MSFT is Microsoft. Keep the response under 3000 characters! Keep sentences simple with minimal fluff. You provide summaries about the stocks based on data in the prompt and dont make any predictions or suggestions on whether people should buy or sell. Try and keep these responses under 3 minute speaking time"},
        {"role": "user",
         "content": prompt}
      ]
    )

    response_text = completion.choices[0].message.content
    
    with open(file_path, "w") as file:
        file.write(str(response_text))
    
    return response_text

def generate_short_form(client, prompts, tickers):
    files = []
    for ticker in tickers:
        completion = client.chat.completions.create(
            model = 'gpt-4o',
            messages=[
                {"role": "developer",
                "content": "You are a professional analyst for stocks which means no emojis. Always read tickers as their full name, ex : AAPL is Apple and MSFT is Microsoft. Please provide a summary for the stocks performance of the day. Try and keep the response to a 1 minute speaking time"},
                {"role":"user",
                "content": prompts[ticker]}
            ]

        )
        response_text = completion.choices[0].message.content
        file_path = f"{ticker}_short_summary.txt"
        with open(file_path, "w") as file:
            file.write(str(response_text))
            
        generate_audio(client, response_text, f"{ticker}_short_audio.mp3")
        files.append(f"{ticker}_short_audio.mp3")
    return files
        

def generate_audio(client, text, file_path):
    tts_response = client.audio.speech.create(
        model="tts-1",
        voice = "echo",
        input = text)
    
    with open(file_path, "wb") as audio_file:
        audio_file.write(tts_response.content)
    
def generate_video(audio_files, video_files, gameplay_files, passed):
    passed = True
    gameplay_num = 0
    audio_num = 0
    vid_num = 0
    starting_point = 0

    while(passed):
        
        if gameplay_num >= len(gameplay_files):
            return

        if audio_num >= len(audio_files):
            return
        
        if vid_num >= len(video_files):
            return
        
        gameplay_clip = VideoFileClip(gameplay_files[gameplay_num]).without_audio()
        audio_clip = AudioFileClip(audio_files[audio_num])
        
        duration = audio_clip.duration
        
        if duration > gameplay_clip.duration:
            gameplay_num+=1
            starting_point = 0
            continue
        
        original_filename = gameplay_files[gameplay_num]
        temp_filename = "Temp.mp4"

        trimmed_gameplay = gameplay_clip.subclipped(starting_point,starting_point+duration)
        final_clip = trimmed_gameplay.with_audio(audio_clip)

        # Save the final output
        final_clip.write_videofile(video_files[vid_num], fps=gameplay_clip.fps, codec="libx264", audio_codec="aac")
        
        
        remaining_gameplay = gameplay_clip.subclipped(starting_point + duration, gameplay_clip.duration)
        remaining_duration = remaining_gameplay.duration
        
        if remaining_duration > 0:
            starting_point = duration
            #remaining_gameplay.write_videofile(temp_filename, fps=gameplay_clip.fps, codec="libx264", preset="ultrafast")
        else:
            return
        
        # Wait for the file to be fully written
        # while not os.path.exists(temp_filename):
        #     time.sleep(0.1)  # Brief wait

        # Once the video is saved, replace the original
        # os.replace(temp_filename, original_filename)
        
        vid_num += 1
        audio_num += 1
        
    
        
def generate_caption(client, prompts):
    for key, prompt in prompts.items():
        
        completion = client.chat.completions.create(
            model='gpt-4o',
            messages=[
                {"role": "developer", 
                "content": "You are a professional analyst for stocks so no emojis. Always read tickers as their full name, ex: AAPL is Apple and MSFT is Microsoft."},
                {"role": "user",
                "content": f"Key:{key}\n{prompt}"}
            ]
            )
        response_text = completion.choices[0].message.content
        
        with open(f"{key}_caption.txt", "w") as file:
            file.write(str(response_text))
            

