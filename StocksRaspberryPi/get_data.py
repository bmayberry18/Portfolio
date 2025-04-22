import yfinance as yf
import utils
import json
import os
from datetime import datetime,timedelta
import pandas as pd



# Gets the daily data for each stock within the stock tickers file
def Get_Daily_Data(tickers):

    # Initialize the dictionary
    all_stock_data = {}

    # For each ticker download the data in the 1d period
    for ticker in tickers:
        stock_data = yf.download(ticker, period='1d')
        all_stock_data[ticker] = stock_data

    return all_stock_data

# Gets each tickers weekly data within the stock tickers file
def Get_Weekly_Data(tickers, today):

    # Initialize the dictionary
    all_weekly_data = {}

    friday = today - timedelta(days=(today.weekday()-4))
    monday = friday - timedelta(days=4)
    print(f"Friday: {friday}\n Monday: {monday}")

    start_date = monday.strftime('%Y-%m-%d')
    end_date = friday.strftime('%Y-%m-%d')
    # For each ticker download the data from the previous week with a one day interval
    for ticker in tickers:
        stock_data = yf.download(ticker, start=start_date,end=end_date, interval='1h')

        weekly_data = pd.DataFrame({
            'Open': [stock_data['Open'].iloc[0]],  # Monday Open
            'Close': [stock_data['Close'].iloc[-1]],  # Friday Close
            'High': [stock_data['High'].max()],  # Maximum high for the week
            'Low': [stock_data['Low'].min()],  # Minimum low for the week
            'Volume': [stock_data['Volume'].sum()]  # Total volume for the week
        }, index=[monday])  # Add the Monday date as the index
        
        #weekly_data = weekly_data.tail(1)

        all_weekly_data[ticker] = weekly_data

    return all_weekly_data
