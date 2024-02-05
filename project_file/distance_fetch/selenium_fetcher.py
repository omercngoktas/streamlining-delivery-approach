import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

def fetch_label(wait, value):
    return wait.until(EC.visibility_of_element_located((By.XPATH, value)))

def send_location(value, latitude, longitude):
    value.send_keys(str(latitude) + "," + str(longitude))

def click_search_button(value):
    value.click()

def fetch_duration_and_distance(latitude1, longitude1, latitude2, longitude2):
    url = f"https://www.google.com/maps/dir/{latitude1},{longitude1}/{latitude2},{longitude2}/data=!4m2!4m1!3e0?entry=ttu"
    
    
    chrome_options = webdriver.ChromeOptions()
    
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")
    driver = webdriver.Chrome(options=chrome_options)
    driver.get(url)
    wait = WebDriverWait(driver, 3)
    try:
        duration = wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[1]")))
        distance = wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[2]/div")))
    
    except Exception as e:
        print(e)
        driver.quit()
        return None, None
    
    return duration.text, distance.text
