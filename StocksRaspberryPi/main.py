import utils
import get_data as data
from datetime import datetime,timedelta
import chat

today = datetime.today()
tickers = utils.read_tickers()

# tickers_data = data.Get_Daily_Data(tickers)
# utils.Save_Data(tickers_data)

# tickers_data = data.Get_Weekly_Data(tickers, today)
# utils.Save_Data(tickers_data, period = 'Weekly')

# prompt = utils.Daily_Prompt(tickers, today)

    
individual_prompts = {}
caption_prompts = {}

for ticker in tickers:
    individual_prompt = utils.Individual_Prompt(ticker, today)
    individual_prompts[ticker] = individual_prompt
    
    caption_prompt = utils.Caption_Prompt(ticker)
    caption_prompts[ticker] = caption_prompt
chat.generate_response(tickers, "", individual_prompts, caption_prompts)
