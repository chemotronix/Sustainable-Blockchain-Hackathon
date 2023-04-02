import {useState, useEffect} from 'react';
import { FGStorage } from '@co2-storage/js-api';
import axios from "axios";
import './App.css';

const authType = "metamask"
const ipfsNodeType = "browser"
const ipfsNodeAddr = "/dns4/web2.co2.storage/tcp/5002/https"
const fgApiUrl = "https://web2.co2.storage"

const fgStorage = new FGStorage({authType: authType, ipfsNodeType: ipfsNodeType, ipfsNodeAddr: ipfsNodeAddr, fgApiHost: fgApiUrl})

function App() {
  const [data, setData] = useState(null);
  const [id, setId] = useState(null);
  const [createdAt, setCreatedAt] = useState(null);
  const [temperature, setTemperature] = useState(null);
  const [pressure, setPressure] = useState(null);
  const [humidity, setHumidity] = useState(null);
  const [altitude, setAltitude] = useState(null);
  const [methane, setMethane] = useState(null);
  const [carbonMonoxide, setCarbonMonoxide] = useState(null);
  const [carbonDioxide, setCarbonDioxide] = useState(null);
  const [totalEmissions, setTotalEmissions] = useState(null);

  useEffect(() => {
    fetchData();
  }, []);

  useEffect(() => {
    {
      data && data.feeds.map((feed, index) => {
        setId(feed.entry_id);
        setCreatedAt(feed.created_at);
        setTemperature(feed.field1);
        setPressure(feed.field2);
        setHumidity(feed.field3);
        setAltitude(feed.field4);
        setMethane(feed.field6);
        setCarbonMonoxide(feed.field6);
        setCarbonDioxide(feed.field7);
        setTotalEmissions(feed.field8);
      })
    }
  }, [data]);

  const fetchData = async () => {
    const result = await axios.get("https://api.thingspeak.com/channels/1803223/feeds.json?results=1");

    setData(result.data);
  };


  const assetElements = [
    {
      "name": "Id",
      "value": id
  },
  {
      "name": "Created At",
      "value": createdAt
  },
  {
      "name": "Temperature",
      "value": temperature
  },
  {
    "name": "Pressure",
    "value": pressure
  },
  {
    "name": "Humidity",
    "value": humidity
  },
  {
    "name": "Altitude",
    "value": altitude
  },
  {
    "name": "Methane",
    "value": methane
  },
  {
    "name": "Carbon Monoxide",
    "value": carbonMonoxide
  },
  {
    "name": "Carbon Dioxide",
    "value": carbonDioxide
  },
  { 
    "name": "Total Emissions Per hour",
    "value": totalEmissions
  }
  ]

  async function AddAsset(){
    let addAssetResponse = await fgStorage.addAsset(
      assetElements,
      {
          parent: null,
          name: "Asset from ChemotronV2",
          description: "Carbon data",
          template: "bafyreid2xwmqdt7hr6ay7xbrpftq64zdazszcw7dzjf7423q2rzygnbftm",    // CID of above template
          filesUploadStart: () => {
              console.log("Upload started")
          },
          filesUploadEnd: () => {
              console.log("Upload finished")
          },
          createAssetStart: () => {
              console.log("Creating asset")
          },
          createAssetEnd: () => {
              console.log("Asset created")
          }
      },
      'sandbox'
        )
        if(addAssetResponse.error != null) {
            console.error(addAssetResponse.error)
            await new Promise(reject => setTimeout(reject, 300));
            process.exit()
        }
  
        console.dir(addAssetResponse.result, {depth: null})
  
        await new Promise(resolve => setTimeout(resolve, 1000));
  
        // Exit program
        // process.exit()
      }

      

  return (
    <div className="App">
            <button onClick={AddAsset}>Add Asset</button>
    </div>
  );
}

export default App;
