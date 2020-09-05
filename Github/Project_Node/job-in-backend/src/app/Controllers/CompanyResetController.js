const UserCompany = require('../Models/UserCompany');
const crypto = require('crypto');
const mailer = require('../../modulos/mailer');

module.exports = {

    async store( req, res ){
        const { email } = req.body;
        const user = await UserCompany.findOne({ email })

        if(!user)
            return res.status(400).send({ error: 'User not found' });

        const token = await crypto.randomBytes(2).toString('hex');
        console.log("TOken gerado: "+token);
        // await mailer.sendMail({
        //     from: 'Job-in <friend.nativejs@gmail.com>',
        //     to: email,
        //     subject: 'Recuperacao de senha',
        //     template: 'Companies/reset',
        //     context: { token },
        // });

        return res.send({ msg: 'Verifique o seu email, lhe foi enviado um e-mail para troca de senha.' });
    }
}