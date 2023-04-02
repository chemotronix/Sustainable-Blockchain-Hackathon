import Head from "next/head";
import Navbar from "../components/Navbar";
import style from "../styles/signedIn.module.css";
import * as RiIcons from "react-icons/ri";
import { useState } from "react";
import Footer from "../components/Footer";
import Link from "next/link";
import connectContract from "../utils/connectContract";
import { toast } from "react-toastify";
import {
    BASE_URL,
    getUniqueId,
  
  } from "../utils/global";

const OffsetCarbon = () => {
  const [showBuying, setShowBuying] = useState(false);
  const buying = "Co2E";
  const [recieverAddress, setRecieverAddress] = useState("");
  const [amount, setAmount] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const toggleBuying = () => {
    setShowBuying(!showBuying);
  };
 
  const clear = () => {
    setAmount("");
  };


  const Offset = async () => {
    setIsLoading(true);
    if (getUniqueId) {
      try {
        const chemContract = connectContract();
  
        if (chemContract) {
          let uid = getUniqueId();
          // let eventDataCID = cid;
  
          const txn = await chemContract.offset(amount, uid);
          console.log("Minting...", txn.hash);
          console.log("Minted -- ", txn);
          setIsLoading(false);
          clear();
          toast.success("Token sent");
        } else {
          console.log("Error getting contract.");
          setIsLoading(false);
        }
      } catch (error) {
        console.log(error, "err");
      }
    }
    
  };

  


  return (
    <div>
      <Head>
        <title>Chemotronix</title>
        <meta name="description" content="Generated by create next app" />
        <link rel="icon" href="/logo.png" />
      </Head>

      <div className={style.bg}></div>
      <main className="flex justify-center ">
        <div className="container px-5">
          <Navbar />

          <div className="mt-24 w-full flex justify-start">
            <Link href={"/signedIn"}>
              <div className="bg-green-800 h-16 rounded-md cursor-pointer px-12 flex items-center">
                <p className=" text-white">Back</p>
              </div>
            </Link>
          </div>

          <div className="mt-16 w-full flex justify-center">
            <div className="flex flex-col items-center w-full md:w-1/2">
              <div className="flex flex-col justify-center mb-7 ">
                <p>Offset carbon credit</p>
                <h1 className="text-4xl md:text-6xl font-bold">
                  How much of carbon would you like to offset
                </h1>
              </div>

              <div className="w-full relative">
                <input
                  type="number"
                  placeholder="Enter amount"
                  className="bg-green-100 pl-36 w-full border-2 border-green-300 cursor-pointer rounded-lg px-8 py-6 flex  justify-center items-center"
                  value={amount}
                  onChange={(e) => setAmount(e.target.value)}
                />
                <div
                  className="absolute top-3 left-10 flex  items-center cursor-pointer"
                  onClick={toggleBuying}
                >
                  <p className="font-bold">{buying}</p>
                  <RiIcons.RiArrowDropDownLine className="text-6xl" />
                </div>  
               
              </div>
              <div className="bg-green-800 h-16 mt-10 rounded-md cursor-pointer px-12 flex items-center">
                <p className=" text-white" onClick={() => Offset()}>
                  {isLoading ? "Loading..." : "Proceed with Offset"}
                </p>
              </div>
            </div>
          </div>
        </div>
      </main>

      <div className="flex flex-col items-center relative">
        <div className="container">
          <Footer></Footer>
        </div>
        <div className="bg-[#E9F4FB] h-24 w-full bottom-0 absolute z-[-1]"></div>
      </div>
    </div>
  );
};

export default OffsetCarbon;