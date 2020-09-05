const UserCompany = require('../Models/UserCompany');
const mailer = require('../../modulos/mailer');
const crypto = require('crypto');

module.exports = {
    
    async store( req, res ){
        const { email , name} = req.body;

        if(await UserCompany.findOne({ email }))
            return res.send({ error: 'Usuario jah existe' });
        
        if(await UserCompany.findOne({ name }))
            return res.send({ error: 'Este nome jah foi cadastrado por uma outra empresa' });

        const userCompanies = await UserCompany.create(req.body);

        userCompanies.password = undefined;
        await mailer.sendMail({
            from: 'Job-in <friend.nativejs@gmail.com>',
            to: email,
            subject: 'Subscricao',
            template: 'Companies/bem_vindo',
            context: { name },
        });
        return res.send({ userCompanies });
    } 
}