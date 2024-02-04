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
    url = "https://www.google.com/maps/dir/"
    chrome_options = webdriver.ChromeOptions()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")
    driver = webdriver.Chrome(options=chrome_options)
    driver.get(url)
    wait = WebDriverWait(driver, 30)
    
    try:
        current_location_label = fetch_label(wait, "/html/body/div[2]/div[3]/div[8]/div[3]/div[1]/div[2]/div/div[3]/div[1]/div[1]/div[2]/div[1]/div/input")
        next_location_label = fetch_label(wait, "/html/body/div[2]/div[3]/div[8]/div[3]/div[1]/div[2]/div/div[3]/div[1]/div[2]/div[2]/div[1]/div/input")
        search_button_label = fetch_label(wait, "/html/body/div[2]/div[3]/div[8]/div[3]/div[1]/div[2]/div/div[3]/div[1]/div[2]/div[2]/button[1]")
        
        
        current_location_label.send_keys(str(latitude1) + "," + str(longitude1))
        next_location_label.send_keys(str(latitude2) + "," + str(longitude2))
        search_button_label.click()
        travel_with_car = fetch_label(wait, "/html/body/div[2]/div[3]/div[8]/div[3]/div[1]/div[2]/div/div[2]/div/div/div/div[2]/button/img")
        travel_with_car.click()

        duration = wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[1]")))
        distance = wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[2]/div")))
    
    except Exception as e:
        print(e)
        driver.quit()
        return None, None
    
    return duration.text, distance.text
