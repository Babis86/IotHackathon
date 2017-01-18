# HHZ IoT Hackathon - Group "Photon Button"

This project is based on the Idea of *physical likes*. Using an internet button, students are able to "like" or "dislike" a lecture by pressing the corresponding button "like" or "dislike". The results are displayed in dashboard on a web UI. Additionally, using a second internet button students are also able to send information when they going to have a coffee, smoke a cigarette, went to have meal, or need to go the bathroom.

The internet button used in this project is the  [Photon button from Particle](https://docs.particle.io/datasheets/photon-datasheet/). It is an programmable internet button equiped with an WLAN module. Pressing on of four button sends an event which then can be processed.

## Implementation

The project is implemented on [Bluemix](https://console.ng.bluemix.net/), IBM's Cloud Platform. Bluemix provides a several services for IoT, web, and mobile application with a quick setup and an easy to use experience.

The overall service consist basically out of two components:

**Watson IoT Service**

The Watson IoT service connects the *Photon Buttons* to the IoT Cloud and is the communication interface between the *HHZ-Photon Service* and the the *Photon Buttons*.  

**HHZ-Photon Service**

The HHZ-Photon is an service which processes the data sensed by the *Photon Buttons* and serves a (web) unser interface (UI) which subsequently dispays the processed data. Thereby, the frontend and the backend part are seperate components:

- HHZ-Photon-Server (implemented in Google Go)
- HHZ-Photon-WI (web interface - implemented in JavaScript/AngularJS)

### Architecture

![architecture](images/architecture.png)

Based on the *Publish/Subscribe Pattern* the *Photon Buttons* are sending their data to an *topic* in the IoT Cloud. The HHZ-Photon-Server subscribes to the corresponding topics and consumes all incoming *json* events. On each incomming message the server component sends the information via *WebSockets* to the the user interface. In the UI the information is displayed in block charts. The UI is served by a lightweight web-server.
