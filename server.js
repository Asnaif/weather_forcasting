const express = require("express");
const mongoose = require("mongoose");
const http = require("http");
const { Server } = require("socket.io"); // WebSockets
const bodyParser = require("body-parser");
const cors = require("cors");
const path = require("path");

mongoose.connect('mongodb+srv://Asnaif:mXxbGmWlKyXJn6AZ@cluster0.17mfx.mongodb.net/')
  .then(() => console.log("✅ MongoDB Connected"))
  .catch(err => console.log("❌ MongoDB Connection Error:", err));

const db = mongoose.connection;
db.on("error", console.error.bind(console, "❌ MongoDB Connection Error:"));
db.once("open", () => console.log("✅ Connected to MongoDB Compass!"));

const sensorSchema = new mongoose.Schema({
  temperature: Number,
  humidity: Number,
  air_quality: Number,
  timestamp: { type: Date, default: Date.now }
}, { collection: "SensorData" });

const SensorData = mongoose.model("SensorData", sensorSchema);

const app = express();
const server = http.createServer(app);

// ✅ Enable CORS for React frontend (usually on port 3000 during development)
const io = new Server(server, {
  cors: {
    origin: "https://bright-aliza-asnaif-bfedfd0f.koyeb.app/", // 🔁 Allow requests from React development server
    methods: ["GET", "POST"]          // 🛠 Allow HTTP methods used by React
  }
});

// ✅ Allow HTTP requests from React frontend (port 3000)
app.use(cors({ origin: "https://bright-aliza-asnaif-bfedfd0f.koyeb.app/" }));

app.use(bodyParser.json());

// 📡 POST: Receive data from Arduino (or any sensor input source)
app.post("/api/sensors", async (req, res) => {
  const { temperature, humidity, air_quality } = req.body;

  // ⚠️ Ensure air_quality is present to avoid incomplete records
  if (air_quality === undefined) {
    return res.status(400).json({ error: "air_quality is missing in request" });
  }

  try {
    // ✅ Save new sensor reading in MongoDB
    const newSensorData = new SensorData({ temperature, humidity, air_quality });
    await newSensorData.save();

    // ⚡ Emit live update to all WebSocket-connected React clients
    io.emit("new_data", newSensorData);

    res.status(200).json({ message: "Data stored successfully!", data: newSensorData });
  } catch (error) {
    console.error("❌ Error storing data:", error);
    res.status(500).json({ error: "Failed to store data" });
  }
});

// 📊 GET: Fetch all stored sensor readings
app.get("/api/sensors", async (req, res) => {
  try {
    const data = await SensorData.find().sort({ timestamp: -1 }); // 🕒 Sorted newest first
    res.status(200).json(data); // 📦 Send to React frontend
  } catch (error) {
    console.error("❌ Error fetching data:", error);
    res.status(500).json({ error: "Failed to fetch data" });
  }
});

// ⚡ WebSocket setup: React frontend connects here for real-time updates
io.on("connection", (socket) => {
  console.log("⚡ New WebSocket Connection:", socket.id);
});

// 🚀 Start server on port 5000 (React usually runs on 3000)
const PORT = 5000;
server.listen(PORT, () => console.log(`🌍 Server running on http://localhost:${PORT}`));
