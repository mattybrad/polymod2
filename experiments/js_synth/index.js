// express server

const fs = require('fs');
const path = require('path');
const express = require('express');
const port = 3000;
const app = express();

app.use('/app', express.static('app'));
app.use(express.static('app'));
app.listen(port, () => console.log(`Example app listening at http://localhost:${port}`))
