import threading
from selenium import webdriver

def fetch_data(url):
    # Initialize WebDriver
    driver = webdriver.Chrome()
    
    # Open the webpage
    driver.get(url)
    
    # Fetch data and perform other operations
    # Example: Extract data from the page
    data = driver.title
    
    # Close the WebDriver
    driver.quit()
    
    return data

def main():
    # List of URLs to fetch data from
    urls = ["https://google.com", "https://google.com", "https://google.com"]
    
    # List to store fetched data
    results = []
    
    # Create threads for each URL
    threads = []
    for url in urls:
        thread = threading.Thread(target=lambda u: results.append(fetch_data(u)), args=(url,))
        threads.append(thread)
        thread.start()
    
    # Wait for all threads to complete
    for thread in threads:
        thread.join()
    
    # Process the fetched data
    for result in results:
        print(result)  # Example: Print the fetched data

if __name__ == "__main__":
    main()
