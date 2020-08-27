// express server

const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const port = 3000;

app.use('/app', express.static('app'));
app.use(express.static('app'));

app.listen(port, () => console.log(`Example app listening at http://localhost:${port}`))
