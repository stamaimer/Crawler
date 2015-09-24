# Crawler
This project has crawlers for newegg, walmart, bestbuy, synnex. These crawlers crawl the menus and merchadises information from E-commerce websites. The main method to get those information is send request to mobile application of that E-commerce websites. Compare to send request to website directly, you'll get json response if you utilize the mobile application. Json is more easier to parse than html if you don't familar with regular expression.(In some case, the website may have a `mobile.example.com` or `m.example.com`. It's also return json or easy to parse) 

The main problems we met in this project are:
- How to crawl the whole website ASAP
- How to avoid our IP be blocked 

To solve the first problem, we rewrite the first version with mlutithread. We write this project with Qt. Because there is no native network and thread library(cross platform) for C++.

To solve the second problem, we bought many proxy IP. We send each request with different proxy IP. But beacuse the `quality`(I don't know how to descirbe it) of these proxy IP is bad. The result is not satisfactory.

The main structure of this project: 

- There is a `JobScheduler` for each crawler. It maintance a job queue and other variables for synchronization between threads. 
- The item of this queue is the object of specific E-commerce website(contains url and request times infomation). 
- There are some threads to poll job queue. If there are jobs left, pop it, do it and process it.
- At the beginning of the program, we'll initial the job queue with the first level menu of that website. In the process of thread deal with jobs, it will add urls parsed from response to job queue.

There are also some details not mentioned in this explanation. 
