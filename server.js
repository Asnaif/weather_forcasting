// const express = require("express");
// const mongoose = require("mongoose");
// const bodyParser = require("body-parser");

// // 🔗 Connect to MongoDB Compass (Change if your MongoDB runs on another server)
// mongoose.connect('mongodb://127.0.0.1:27017/weatherDB')
//     .then(() => console.log("✅ MongoDB Connected"))
//     .catch(err => console.log("❌ MongoDB Connection Error:", err));

// const db = mongoose.connection;
// db.on("error", console.error.bind(console, "❌ MongoDB Connection Error:"));
// db.once("open", () => console.log("✅ Connected to MongoDB Compass!"));

// // 🌡️ Define Schema for Sensor Data
// const sensorSchema = new mongoose.Schema({
//   temperature: Number,
//   humidity: Number,
//   air_quality: Number,
//   timestamp: { type: Date, default: Date.now },
// });

// // 📌 Create Model
// const SensorData = mongoose.model("SensorData", sensorSchema);

// const app = express();
// app.use(bodyParser.json());

// // 📡 API to Receive Data from Arduino
// app.post("/api/sensors", async (req, res) => {
//   try {
//     const { temperature, humidity, air_quality } = req.body;

//     // 📝 Store data in MongoDB
//     const newSensorData = new SensorData({ temperature, humidity, air_quality });
//     await newSensorData.save();

//     console.log("✅ Data Stored:", newSensorData);
//     res.status(200).json({ message: "Data stored successfully!", data: newSensorData });
//   } catch (error) {
//     console.error("❌ Error storing data:", error);
//     res.status(500).json({ error: "Failed to store data" });
//   }
// });

// // 🚀 Start Server
// const PORT = 5000;
// app.listen(PORT, () => console.log(`🌍 Server running on http://localhost:${PORT}`));



const express = require("express");
const mongoose = require("mongoose");
const http = require("http");
const { Server } = require("socket.io"); // WebSockets
const bodyParser = require("body-parser");
const cors = require("cors");

// 🔗 Connect to MongoDB
mongoose.connect('mongodb://127.0.0.1:27017/weatherDB')
    .then(() => console.log("✅ MongoDB Connected"))
    .catch(err => console.log("❌ MongoDB Connection Error:", err));

const db = mongoose.connection;
db.on("error", console.error.bind(console, "❌ MongoDB Connection Error:"));
db.once("open", () => console.log("✅ Connected to MongoDB Compass!"));



// 🌡️ Define Schema for Sensor Data
const sensorSchema = new mongoose.Schema({
    temperature: Number,
    humidity: Number,
    air_quality: Number, // ✅ Ensure this exists
    timestamp: { type: Date, default: Date.now }
  },
  { collection: "SensorData" }
);
  
  // 📌 Create Model
  const SensorData = mongoose.model("SensorData", sensorSchema);

const app = express();
const server = http.createServer(app); // Create HTTP Server
const io = new Server(server, { cors: { origin: "*" } }); // Enable WebSockets

app.use(cors()); // Allow frontend to connect
app.use(bodyParser.json());

// 📡 API to Receive Data from Arduino
app.post("/api/sensors", async (req, res) => {
  console.log("Received Data:", req.body);  // 🔍 Debug incoming data
  const { temperature, humidity, air_quality } = req.body;
  
  if (air_quality === undefined) {
      return res.status(400).json({ error: "air_quality is missing in request" });
  }

  try {
      const newSensorData = new SensorData({ temperature, humidity, air_quality });
      await newSensorData.save();
      console.log("✅ Data Stored:", newSensorData);

      // ⚡ Emit to WebSocket Clients
      io.emit("new_data", newSensorData);

      res.status(200).json({ message: "Data stored successfully!", data: newSensorData });
  } catch (error) {
      console.error("❌ Error storing data:", error);
      res.status(500).json({ error: "Failed to store data" });
  }
});


// 📊 API to Fetch All Sensor Data
app.get("/api/sensors", async (req, res) => {
  try {
    const data = await SensorData.find().sort({ timestamp: -1 });
    res.status(200).json(data);
  } catch (error) {
    console.error("❌ Error fetching data:", error);
    res.status(500).json({ error: "Failed to fetch data" });
  }
});

// ⚡ WebSocket Connection
io.on("connection", (socket) => {
  console.log("⚡ New WebSocket Connection:", socket.id);
});

// 🚀 Start Server
const PORT = 5000;
server.listen(PORT, () => console.log(`🌍 Server running on http://localhost:${PORT}`));

