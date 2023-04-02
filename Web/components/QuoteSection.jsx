import { motion } from "framer-motion";
import Marquee from "react-fast-marquee";
import Image from "next/image";
import React from "react";
import City from "../images/city.png";
import Q1 from "../images/1.png";
import Q2 from "../images/2.png";
import Q3 from "../images/3.png";
import Q4 from "../images/4.png";

function QuoteSection() {
  return (
    <div className="flex items-center justify-center mt-10">
      <div className="flex items-center flex-wrap mx-auto">
        <motion.div
          initial={{ opacity: 0, y: -40, x: -40 }}
          whileInView={{ opacity: 1, y: 0, x: 0 }}
          transition={{ duration: 0.5, delay: 0.5 }}
          className="fle flex-co items-cnter"
        >
          <Marquee speed={100} gradient={false}>
          <div className="flex flex-row mr-auto items-center w-screen">
            <Image src={Q4} className="w-48 rounded"/>
            <Image src={Q2} className="w-48 rounded"/>
            <Image src={Q3} className="w-48 rounded"/>
            <Image src={Q1} className="w-48 rounded"/>
            <Image src={City} className="w-48 rounded"/>

          </div>
          </Marquee>

          {/* <div className="mt-20">
            <Image src={City} />
          </div> */}
        </motion.div>
        <motion.div
          initial={{ opacity: 0, y: 40, x: 40 }}
          whileInView={{ opacity: 1, y: 0, x: 0 }}
          transition={{ duration: 0.5, delay: 0.5 }}
          className="flex items-center flex-col ml-auto"
        >
        </motion.div>
      </div>
    </div>
    
  );
}

export default QuoteSection;
