const mongoose = require('mongoose');
const bcrypt = require('bcryptjs');

const UserSchema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
    },
    email: {
        type: String,
        required: true,
        unique: true,
        lowercase: true,
    },
    password: {
        type: String,
        required: true,
        selec: false,
    }
})

UserSchema.pre('save', async function( next ){
    const hash = await bcrypt.hash(this.password, 5);
    this.password = hash;
    next();
});

module.exports = mongoose.model('User', UserSchema);