using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using MedicalDataManagerModel;

namespace ActionService.Controllers
{
    public class ActionsController : ApiController
    {
        // GET: api/Actions
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET: api/Actions/5
        public string Get(string deviceId, int tokenId)
        {
            return string.Format("{0} - {1}",deviceId,tokenId);
        }
        [HttpGet]
        public string OnActionReceive(int actionId, string payload, DateTime timeStamp, string devGuid)
        {
            var provider = new MedicalDataManagerModelProvider();
            provider.SaveAction(actionId, payload, timeStamp, devGuid);
            return "Hello World";
        }

        [HttpGet]
        public string OnAlarmReceived(string payLoad)
        {
            var provider = new MedicalDataManagerModelProvider();
            return provider.ReturnAlarmdata(payLoad);
        }

        [HttpGet]
        public string OnRequestCompartmentMedPlan(string deviceId, int compartmentId)
        {
            var provider = new MedicalDataManagerModelProvider();
            return provider.ReturnMedPlan(deviceId, compartmentId);
        }

        // POST: api/Actions
        public void Post([FromBody]string value)
        {
        }

        // PUT: api/Actions/5
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE: api/Actions/5
        public void Delete(int id)
        {
        }
    }
}
