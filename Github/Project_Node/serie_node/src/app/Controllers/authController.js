const express = require('express');
const User = require('../models/User');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const authCOnfig = require('../../Config/auth.json');
const crypto = require('crypto');
const mailer = require('../../modulos/mailer')

const router = express.Router()

function gerenareToken(params = {}){
    return jwt.sign(params, authCOnfig.secret, { expiresIn: 86400 });
}


router.post('/register', async( req, res ) => {
    const { email } = req.body;
    try{
        if(!await User.find({ email }))
            return res.status(400).send({ error: 'User already exists' });
        const user = await User.create(req.body);

        user.password = undefined;
        return res.send({ user, token: gerenareToken({ id: user.id }) });
    } catch( err ) {
        return res.status(400).send({ error: 'Registration failed' })
    }
})

router.post('/authenticate', async( req, res, )=> {
    const { email, password } = req.body;
    const user = await User.findOne({ email }).select('+password');

    if(!user)
        return res.status(400).send({ error: 'User not found' });

    if(!await bcrypt.compare(password, user.password))
        return res.status(400).send({ error: 'Invalid password' });

    user.password = undefined;


    return res.send({ user, token: gerenareToken({ id: user.id }) });
});

router.post('/forgot_password', async ( req, res ) => {
    const { email } = req.body;

    try{

        const user = await User.findOne({ email })

        if(!user)
            return res.status(400).send({ error: 'User not found' });

        const token = crypto.randomBytes(20).toString('hex');
        
        const now = new Date();

        now.setHours(now.getHours() + 1);

        await User.findByIdAndUpdate(user.id, {
            '$set': {
                passwordResetToken: token,
                passwordResetExpires: now,
            }
        });
        
        mailer.sendMail({
            to: email,
            from: 'friend_native@gmail.com',
            template: 'auth/forgot',
            context: { token },
        }, ( err ) => {
            if(err){
                console.log('\n Erro:'+err);
                return res.status(400).send({ error: 'Cannot send forgot password email' });
            }
            return res.send();
        })

    } catch{( err ) => { 
        res.status(400).send({ error: 'error on forgot password, try again' })
    }};
})

router.post('/reset_password', async( req, res ) => {
    const { email, token, password } = req.body

    try{
        
        const user = await User.findOne({ email })
            .select('+passwordResetToken passwordResetExpires');


        if(!user)
            return res.status(400).send({ error: 'User not found' })
        
        if(token !== user.passwordResetToken)
            return res.status(400).send({ error: 'Token invalid' })

        const now = new Date();
        if(now > user.passwordResetExpires)
            return res.status(400).send({ error: 'Token expired, generatte a new one' })

        user.password = password;
        await user.save();
        res.send();

    } catch ( err ) {
        res.status(400).send({ error: 'Cannot reset password, try again' })
    }
})
module.exports = app => app.use('/auth', router);