const mongoose = require('mongoose');
const bcrypt = require('bcryptjs');

const UserCompanySchema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
    },
    email: {
        type: String,
        lowercase: true,
        required: true,
    },
    password: {
        type: String,
        required: true,
    }
}, {
    timestamps: true,
});

UserCompanySchema.pre('save', async function( next ){
    const hash = await bcrypt.hash(this.password, 10);
    this.password = hash;
    next();
});

module.exports = mongoose.model('UserCompany', UserCompanySchema);