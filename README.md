# About Chevtrox

Chevtrox is a unique blend of IoT, A.I and Blockchain, which fosters the reduction of carbon emissions in Africa, allowing users to trade carbon credits. 

A company that wishes to use the services of Chevtrox receives an IoT device, this IoT device has a unique ID that is registered on the blockchain. With this unique ID, users and companies can track their carbon emissions which are funnelled to co2.storage by Filecoin and are processed in the blockchain such that the carbon credits associated with the device are burned. With the unique ID, users can transfer and share carbon credits(**an ERC-20 token**) among themselves without needing to remember their lengthy wallet addresses.

The carbon emissions data from the IoT device is saved to co2.storage by Filecoin, instead of traditional cloud storage, to avoid a single point of failure and maintain transparency. The data is to remain open and **autioned on Bigdata.io**

The data from the IoT device is also analyzed using A.I such that stakeholders can forecast carbon emissions and take actions 
to gradually reduce it.

# About C02 Storage Handler

Our Co2 storage handler handles interaction between the IoT and co2.storage, we stream our IoT data and funnel it to co2.storage with a custom built template.

## Chevtrox Custom Schema

```
{
  "Id": {
      "type": "int",
      "mandatory": true
    },
    "Created At": {
      "type": "datetime",
      "mandatory": true
    },
    "Temperature": {
      "type": "float",
      "mandatory": true
    },
    "Humidity": {
      "type": "float",
      "mandatory": true
    },
    "Pressure": {
      "type": "float",
      "mandatory": true
    },
    "Altitude": {
      "type": "float",
      "mandatory": true
    },
    "Carbon Monoxide": {
      "type": "float",
      "mandatory": true
    },
    "Methane": {
      "type": "float",
      "mandatory": true
    },
    "Carbon Dioxide": {
      "type": "float",
      "mandatory": true
    },
    "Total Emissions Per hour": {
      "type": "float",
      "mandatory": true
    }
  }

```


# Blockchain technologies deployed at Hedera

Hedera enabled us to build a scalable blockchain ecosystem, with minimal transaction fees. Hedera was used to create a token, which essentially serves as carbon credits for users to trade on the blockchain, enabling carbon emitters to effectively offset their carbon emissions as they were tracked by the IoT device.


# Open Data at Bigdata.io
The data collected from the Chemotron IoT devices includes various climate details such as humidity, temperature, pressure, Carbon Dioxide(CO2), Carbon Monoxide (CO), and methane (Ch4). The data is collected at regular intervals and sent to **co2.storage** through our co2 storage handler service for storage and Futher analysis.

The data is stored in a time series format, where each row represents a specific time point and contains information about the climate conditions at that time. In addition, the data includes information about the specific location of each IoT device, which could be useful for spatial analysis.

To make the dataset always available, we auction the data on **Bigdata.io**, along with documentation describing the data format and any relevant metadata. We also plan to provide tools and resources to help users work with the data, such as code samples, tutorials, and APIs.

We believe that this dataset will be a valuable resource for researchers, data scientists, and anyone interested in understanding climate conditions and their impact on the environment. We remain committed to maintaining the quality and integrity of the data and are excited to make it available.

# IoT Images

The IoT device Prototype

![The_IoT_device](ImageGallery/2.jpeg)

IoT sending data live to Co2storage through our Co2 Storage Handler

![IoT_sending_data_live_to__co2storage_through_handler](ImageGallery/3.jpeg)

# Major Blockchain Technologies Used

![Co2_Storage](readme_images/co2_storage.png)

![Hedera](readme_images/hedera.png)

![Filecoin](readme_images/filecoin.png)

![Big Data Exchange](readme_images\bde.png)