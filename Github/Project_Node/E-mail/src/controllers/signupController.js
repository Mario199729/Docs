const User = require('../models/signup');
const transporter = require('../modulos/mailer');

module.exports = {
    async store( req, res ){
        const { email, password, name } = req.body;
        try {

            if(!await User.find({ email }))
                return res.status(400).send({ error: 'Usuario ja existe' });

            const user = await User.create(req.body);

            transporter.sendMail({
                from: 'Job-in <friend.nativejs@gmail.com>',
                to: email,
                template: '/confirm',
                subject: 'Confirm your password.',
                context: {name,  password }
            }).then((msg) => { console.log(msg) }).catch(( err ) => { console.log(err) });
            return res.send({ user });

        } catch( err ){
            console.log('erro: '+err);
            return res.status(400).send({ error: 'Houve um erro ao tentar cadastrar' })
        }

    }
}