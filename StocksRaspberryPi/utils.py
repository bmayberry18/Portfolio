# Brooklyn Mayberry
# February 1st, 2025
# Utility file with functions that are used multiple times throughout the project

import os
import json
import pandas as pd

Stock_Data_Folder = 'Stock_Data'

# Stock Ticker File Name
stock_tickers_file = 'stock_tickers.txt'

# Takes in a file name
# opens the file in read mode and strips/splits based on commas
# example data: AAPL,MSFT,NVDA...
# returns the tickers that it has read in
def read_tickers():
    with(open(stock_tickers_file, 'r') as f):
        tickers = f.read().strip().split(',')
    return tickers


# Saves the ticker data to its own json file
# tickers_data needs to be a dictionary with each key being the ticker name
# creates a new file within the sub folder for each stock
# saves the data as a json file
def Save_Data(tickers_data, period = 'Daily'):


    for ticker, data in tickers_data.items():

        data_dict = data.to_dict(orient='records')

        if period == 'Daily':

            file_name = os.path.join(Stock_Data_Folder, f"{ticker}_data_day.json")
        elif period == 'Weekly':
            file_name = os.path.join(Stock_Data_Folder, f"{ticker}_data_week.json")

        with open(file_name, 'w') as f:
            json.dump(data_dict, f, indent=4)


def Extract_Data(ticker, period = 'Daily'):
    if period == 'Daily':
        data = read_json_data(f"{ticker}_data_day.json")
    elif period == 'Weekly':
        data = read_json_data(f"{ticker}_data_week.json")

    stock_info = data[0]

    open_price = stock_info['Open']
    close_price = stock_info['Close']
    high_price = stock_info['High']
    low_price = stock_info['Low']
    volume = stock_info['Volume']

    return open_price, close_price, high_price, low_price, volume


def read_json_data(file_name):
    file_path = os.path.join(Stock_Data_Folder, file_name)

    with open(file_path, 'r') as file:
        data = json.load(file)
    return data


def Daily_Prompt(tickers, today):


    prompt = f"Create a tiktok script summarizing the stock performance of the following stocks for the day {today.strftime('%m-%d-%Y')}. Keep this script under 3500 characters! This script should be readable when converting from text to speech via AI. Convert all numbers to write them out word by word including putting the unit of the value spelled out. Ensure to keep it under 3 minutes and include the following data points, The opening and closing price for today, the price change in both raw and percentage, the highest and lowest price of the day, the trading volume for the day and an engaging line with a positive or negative sentiment about the stocks performace. Keep the tone semi professional, but fun and edgy like a radio show. Make sure to include dollar signs for any numbers you provide! Here are all of the stocks I want you to use for this script:\n"
    for ticker in tickers:
        open, close, high, low, volume = Extract_Data(ticker)
        prompt += f"{ticker}, Open: {open}, Close: {close}, High: {high}, Low: {low}\n\n"

    prompt += "Be sure to keep each segment short and snappy. End with a fun edgy and engaging call-to-action encouraging viewers to like and follow for stock information!"
    return prompt

def Individual_Prompt(ticker, today):
    
    prompt = f"Create a tiktok script summarizing the stock performance of the following stock for the day (month-day-year){today.strftime('%m-%d-%Y')}. This script should be readable when converting from text to speech via AI. Convert all numbers to write them out word by word including putting the unit of the value spelled out. Ensure to keep it under 2 minutes and include the following data points, The opening and closing price for today, the price change in both raw and percentage, the highest and lowest price of the day, the trading volume for the day and an engaging line with a positive or negative sentiment about the stocks performace. Keep the tone semi professional, but fun and edgy like a radio show. Make sure to include dollar signs for any numbers you provide! Here is the stock I want you to use for this script:\n"

    open, close, high, low, volume = Extract_Data(ticker)
    prompt += f"{ticker}, Open: {open}, Close: {close}, High: {high}, Low: {low}, Volume: {volume}\n\n"
    prompt += f"Be sure to include a pun related to the stocks performance. End with a quick engaging call-to-action eouraging viewers to like and follow for stock information! And encourage the audience to comment any additional stock information they would like to see!"
    return prompt

def Caption_Prompt(ticker, summary = False):
    prompt = f"Write a tiktok caption that quickly summarizes the data for the ticker. Encourage viewers to engage with the content by liking, commenting, following and sharing. Be sure to include at least 3 relevant hashtags"
    
    if summary == False:
        open, close, high, low, volume = Extract_Data(ticker)
        prompt += f"{ticker}, Open: {open}, Close: {close}, High: {high}, Low: {low}, Volume: {volume}\n\n"
    else:
        for t in ticker:
            open, close, high, low, volume = Extract_Data(t)
            prompt += f"{ticker}, Open: {open}, Close: {close}, High: {high}, Low: {low}, Volume: {volume}\n\n"
    
    return prompt
            
    
        
    
    
    


